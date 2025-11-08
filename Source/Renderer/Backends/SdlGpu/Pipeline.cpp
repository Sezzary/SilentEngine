#include "Framework.h"
#include "Renderer/Backends/SdlGpu/Pipeline.h"

#include "Application.h"
#include "Renderer/Backends/SdlGpu/PipelineConfig.h"
#include "Renderer/Common/Enums.h"
#include "Services/Filesystem.h"

using namespace Silent::Services;

namespace Silent::Renderer
{
    PipelineManager::~PipelineManager()
    {
        for (auto [keyHash, pipeline] : _pipelines)
        {
            SDL_ReleaseGPUGraphicsPipeline(_device, pipeline);
        }
    }

    void PipelineManager::Initialize(SDL_Window& window, SDL_GPUDevice& device)
    {
        _device = &device;

        for (const auto& pipelineConfig : PIPELINE_CONFIGS)
        {
            InitializeGraphicsPipeline(window, pipelineConfig);
        }
    }

    void PipelineManager::Bind(SDL_GPURenderPass& renderPass, RenderStage renderStage, BlendMode blendMode)
    {
        int pipelineHash = GetPipelineHash(renderStage, Debug::g_Work.EnableWireframeMode ? BlendMode::Wireframe : blendMode);
        SDL_BindGPUGraphicsPipeline(&renderPass, _pipelines[pipelineHash]);
    }

    void PipelineManager::InitializeGraphicsPipeline(SDL_Window& window, const PipelineConfig& config)
    {
        // Load vertex shader.
        auto* vertShader = LoadShader(config.VertexShaderName,
                                      config.VertShaderSamplerCount, config.VertShaderStorageTexCount,
                                      config.VertShaderStorageBufferCount, config.VertShaderUniBufferCount);
        if (vertShader == nullptr)
        {
            throw std::runtime_error(fmt::format("Failed to create vertex shader `{}`.", config.VertexShaderName));
        }

        // Load fragment shader.
        auto* fragShader = LoadShader(config.FragmentShaderName,
                                      config.FragShaderSamplerCount, config.FragShaderStorageTexCount,
                                      config.FragShaderStorageBufferCount, config.FragShaderUniBufferCount);
        if (fragShader == nullptr)
        {
            throw std::runtime_error(fmt::format("Failed to create fragment shader `{}`.",  config.FragmentShaderName));
        }

        // @todo Post-process pipelines don't need every variant.
        // Create pipelines with blend mode variants.
        for (int i = 0; i < (int)BlendMode::Count; i++)
        {
            auto colorTargetDescs = config.ColorTargetDescs;
            colorTargetDescs.push_back(SDL_GPUColorTargetDescription
            {
                .format      = SDL_GetGPUSwapchainTextureFormat(_device, &window),
                .blend_state = PIPELINE_BLEND_MODE_COLOR_TARGETS[i]
            });

            auto pipelineInfo = SDL_GPUGraphicsPipelineCreateInfo
            {
                .vertex_shader      = vertShader,
                .fragment_shader    = fragShader,
                .vertex_input_state =
                {
                    .vertex_buffer_descriptions = config.VertBufferDescs.data(),
                    .num_vertex_buffers         = (uint)config.VertBufferDescs.size(),
                    .vertex_attributes          = config.VertBufferAttribs.data(),
                    .num_vertex_attributes      = (uint)config.VertBufferAttribs.size()
                },
                .primitive_type   = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
                .rasterizer_state = SDL_GPURasterizerState
                {
                    .fill_mode = ((BlendMode)i == BlendMode::Wireframe) ? SDL_GPU_FILLMODE_LINE : SDL_GPU_FILLMODE_FILL
                },
                .target_info = SDL_GPUGraphicsPipelineTargetInfo
                {
                    .color_target_descriptions = colorTargetDescs.data(),
                    .num_color_targets         = (uint)colorTargetDescs.size()
                }
            };

            // Create pipeline variant.
            int piplineHash         = GetPipelineHash(config.Stage, (BlendMode)i);
            _pipelines[piplineHash] = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
            if (_pipelines[piplineHash] == nullptr) 
            {
                throw std::runtime_error(fmt::format("Failed to create graphics pipeline for render stage {}, blend mode {}: {}",
                                                     (int)config.Stage, i, SDL_GetError()));
            }
        }

        // Free shaders.
        SDL_ReleaseGPUShader(_device, vertShader);
        SDL_ReleaseGPUShader(_device, fragShader);
    }

    SDL_GPUShader* PipelineManager::LoadShader(const std::string& filename, uint samplerCount, uint storageTexCount, uint storageBufferCount, uint uniBufferCount)
    {
        // Define shader stage.
        auto stage = SDL_GPUShaderStage{};
        if (SDL_strstr(filename.c_str(), ".vert"))
        {
            stage = SDL_GPU_SHADERSTAGE_VERTEX;
        }
        else if (SDL_strstr(filename.c_str(), ".frag"))
        {
            stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        }
        else
        {
            Debug::Log("Invalid shader stage.", Debug::LogLevel::Error);
            return nullptr;
        }

        const auto& fs = g_App.GetFilesystem();

        // Define shader properties.
        char        fullPath[256];
        auto        formatFlags      = SDL_GetGPUShaderFormats(_device);
        auto        activeFormatFlag = (SDL_GPUShaderFormat)SDL_GPU_SHADERFORMAT_INVALID;
        const char* entryPoint       = nullptr;

        if (formatFlags & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.spv", (fs.GetShadersDirectory() / filename).string().c_str());
            activeFormatFlag = SDL_GPU_SHADERFORMAT_SPIRV;
            entryPoint = "main";
        }
        else if (formatFlags & SDL_GPU_SHADERFORMAT_MSL)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.msl", (fs.GetShadersDirectory() / filename).string().c_str());
            activeFormatFlag = SDL_GPU_SHADERFORMAT_MSL;
            entryPoint = "main0";
        }
        else if (formatFlags & SDL_GPU_SHADERFORMAT_DXIL)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.dxil", (fs.GetShadersDirectory() / filename).string().c_str());
            activeFormatFlag = SDL_GPU_SHADERFORMAT_DXIL;
            entryPoint = "main";
        }
        else
        {
            Debug::Log("Unrecognized backend shader format.", Debug::LogLevel::Error);
            return nullptr;
        }

        // Load shader code.
        size_t codeSize = 0;
        void*  code     = SDL_LoadFile(fullPath, &codeSize);
        if (code == nullptr)
        {
            Debug::Log(fmt::format("Failed to load shader `{}`: {}", fullPath, SDL_GetError()), Debug::LogLevel::Error);
            return nullptr;
        }

        auto shaderInfo = SDL_GPUShaderCreateInfo
        {
            .code_size            = codeSize,
            .code                 = (const uint8*)code,
            .entrypoint           = entryPoint,
            .format               = activeFormatFlag,
            .stage                = stage,
            .num_samplers         = samplerCount,
            .num_storage_textures = storageTexCount,
            .num_storage_buffers  = storageBufferCount,
            .num_uniform_buffers  = uniBufferCount
        };

        // Create shader.
        auto* shader = SDL_CreateGPUShader(_device, &shaderInfo);
        if (shader == nullptr)
        {
            Debug::Log(fmt::format("Failed to create shader `{}`: {}", fullPath, SDL_GetError()));
        }
        SDL_free(code);

        return shader;
    }

    int PipelineManager::GetPipelineHash(RenderStage renderStage, BlendMode blendMode)
    {
        return ((int)renderStage * (int)BlendMode::Count) + (int)blendMode;
    }
}

#include "Framework.h"
#include "Renderer/Backends/SdlGpu/Pipeline.h"

#include "Application.h"
#include "Renderer/Backends/SdlGpu/PipelineConfig.h"
#include "Renderer/Common/Enums.h"
#include "Services/Filesystem.h"

using namespace Silent::Services;

namespace Silent::Renderer
{
    static const auto PIPELINE_BLEND_MODE_COLOR_TARGETS = std::vector<SDL_GPUColorTargetBlendState>
    {
        // Opaque.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Alpha.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Fast alpha. Requires `IsFastAlpha` shader uniform set to `true`.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Multiply.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Add.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Subtract.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_SUBTRACT,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_SUBTRACT,
            .enable_blend          = true
        }
    };

    PipelineManager::~PipelineManager()
    {
        for (auto* pipeline : _pipelines)
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

    void PipelineManager::Bind(SDL_GPURenderPass& renderPass, PipelineType pipelineType, BlendMode blendMode)
    {
        int pipelineIdx = GetPipelineIdx(pipelineType, blendMode, g_DebugData.EnableWireframeMode);
        SDL_BindGPUGraphicsPipeline(&renderPass, _pipelines[pipelineIdx]);
    }

    void PipelineManager::InitializeGraphicsPipeline(SDL_Window& window, const PipelineConfig& config)
    {
        // Load vertex shader.
        auto* vertShader = LoadShader(config.VertexShaderName,
                                      config.VertShaderSamplerCount, config.VertShaderStorageTexCount,
                                      config.VertShaderStorageBufferCount, config.VertShaderUniBufferCount);
        if (vertShader == nullptr)
        {
            throw std::runtime_error("Failed to create vertex shader `" + config.VertexShaderName + "`.");
        }

        // Load fragment shader.
        auto* fragShader = LoadShader(config.FragmentShaderName,
                                      config.FragShaderSamplerCount, config.FragShaderStorageTexCount,
                                      config.FragShaderStorageBufferCount, config.FragShaderUniBufferCount);
        if (fragShader == nullptr)
        {
            throw std::runtime_error("Failed to create fragment shader `" + config.FragmentShaderName + "`.");
        }

        // Create pipeline variants for type, blend mode, and fill mode.
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < (int)BlendMode::Count; j++)
            {
                auto colorTargetDescs = config.ColorTargetDescs;
                colorTargetDescs.push_back(SDL_GPUColorTargetDescription
                {
                    .format      = SDL_GetGPUSwapchainTextureFormat(_device, &window),
                    .blend_state = PIPELINE_BLEND_MODE_COLOR_TARGETS[j]
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
                        .fill_mode = (i == 0) ? SDL_GPU_FILLMODE_FILL : SDL_GPU_FILLMODE_LINE
                    },
                    .target_info = SDL_GPUGraphicsPipelineTargetInfo
                    {
                        .color_target_descriptions = colorTargetDescs.data(),
                        .num_color_targets         = (uint)colorTargetDescs.size()
                    }
                };

                // Create pipeline variant.
                int piplindIdx = GetPipelineIdx(config.Type, (BlendMode)j, i);
                _pipelines[piplindIdx] = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
                if (_pipelines[piplindIdx] == nullptr) 
                {
                    throw std::runtime_error("Failed to create graphics pipeline type " + std::to_string((int)config.Type) +
                                             ", blend mode " + std::to_string(j) +
                                             " for " + (g_DebugData.EnableWireframeMode ? "wireframe mode" : "fill mode") +
                                             ": " + std::string(SDL_GetError()));
                }
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
            Log("Invalid shader stage.", LogLevel::Error);
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
            Log("Unrecognized backend shader format.", LogLevel::Error);
            return nullptr;
        }

        // Load shader code.
        size_t codeSize = 0;
        void*  code     = SDL_LoadFile(fullPath, &codeSize);
        if (code == nullptr)
        {
            Log("Failed to load shader `" + std::string(fullPath) + "`: " + SDL_GetError(), LogLevel::Error);
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
            Log("Failed to create shader: " + std::string(SDL_GetError()));
        }
        SDL_free(code);

        return shader;
    }

    int PipelineManager::GetPipelineIdx(PipelineType pipelineType, BlendMode blendMode, bool isWireframe)
    {
        return (((int)pipelineType * (int)BlendMode::Count) * 2) + ((int)blendMode * 2) + (int)isWireframe;
    }
}

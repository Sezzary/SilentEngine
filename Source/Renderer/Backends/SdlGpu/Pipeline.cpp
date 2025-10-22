#include "Framework.h"
#include "Renderer/Backends/SdlGpu/Pipeline.h"

#include "Application.h"
#include "Services/Filesystem.h"

using namespace Silent::Services;

namespace Silent::Renderer
{
    // @todo Temp.
    struct BufferVertex
    {
        Vector3 Position = Vector3::Zero;
        Color   Col      = Color::Black;
    };
    struct BufferPositionTextureVertex
    {
        float x, y, z;
        float u, v;
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

        // @todo Vectors won't compile as part of initialiser list.
        auto prim2dPipelineConfig = PipelineConfig
        {
            .Type                     = PipelineType::Primitive2d,
            .VertexShaderName         = "2dPrimitive.vert",
            .FragmentShaderName       = "2dPrimitive.frag",
            .FragShaderUniBufferCount = 1
        };
        prim2dPipelineConfig.VertBufferDescs =
        {
            SDL_GPUVertexBufferDescription
            {
                .slot               = 0,
                .pitch              = sizeof(BufferVertex),
                .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0
            }
        };
        prim2dPipelineConfig.VertBufferAttribs =
        {
            SDL_GPUVertexAttribute
            {
                .location    = 0,
                .buffer_slot = 0,
                .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset      = 0
            },
            SDL_GPUVertexAttribute
            {
                .location    = 1,
                .buffer_slot = 0,
                .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                .offset      = sizeof(float) * 3
            }
        };
        prim2dPipelineConfig.ColorTargetDescs =
        {
            SDL_GPUColorTargetDescription
            {
                .format      = SDL_GetGPUSwapchainTextureFormat(_device, &window),
                .blend_state = SDL_GPUColorTargetBlendState
                {
                    .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .color_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .enable_blend          = true
                }
            }
        };
        InitializeGraphicsPipeline(window, prim2dPipelineConfig);

        auto texPipelineConfig = PipelineConfig
        {
            .Type                   = PipelineType::Primitive2dTextured,
            .VertexShaderName       = "TexturedQuad.vert",
            .FragmentShaderName     = "TexturedQuad.frag",
            .FragShaderSamplerCount = 1
        };
        texPipelineConfig.VertBufferDescs =
        {
            SDL_GPUVertexBufferDescription
            {
                .slot               = 0,
                .pitch              = sizeof(BufferPositionTextureVertex),
                .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0
            }
        };
        texPipelineConfig.VertBufferAttribs =
        {
            SDL_GPUVertexAttribute
            {
                .location    = 0,
                .buffer_slot = 0,
                .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset      = 0
            },
            SDL_GPUVertexAttribute
            {
                .location    = 1,
                .buffer_slot = 0,
                .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                .offset      = sizeof(float) * 3
            }
        };
        texPipelineConfig.ColorTargetDescs =
        {
            SDL_GPUColorTargetDescription
            {
                .format      = SDL_GetGPUSwapchainTextureFormat(_device, &window),
                .blend_state = SDL_GPUColorTargetBlendState
                {
                    .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .color_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .enable_blend          = true
                }
            }
        };
        InitializeGraphicsPipeline(window, texPipelineConfig);
    }

    void PipelineManager::Bind(SDL_GPURenderPass& renderPass, PipelineType pipelineType)
    {
        SDL_BindGPUGraphicsPipeline(&renderPass, _pipelines[(int)pipelineType]);
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
                .fill_mode = SDL_GPU_FILLMODE_FILL
            },
            .target_info = SDL_GPUGraphicsPipelineTargetInfo
            {
                .color_target_descriptions = config.ColorTargetDescs.data(),
                .num_color_targets         = (uint)config.ColorTargetDescs.size()
            }
        };

        // Create pipeline.
        _pipelines[(int)config.Type] = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
        if (_pipelines[(int)config.Type] == nullptr) 
        {
            throw std::runtime_error("Failed to create graphics pipeline type " + std::to_string((int)config.Type) + ": " + std::string(SDL_GetError()));
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

        // Define shader properties.
        char        fullPath[256];
        auto        formatFlags = SDL_GetGPUShaderFormats(_device);
        auto        formatFlag  = (SDL_GPUShaderFormat)SDL_GPU_SHADERFORMAT_INVALID;
        const char* entryPoint  = nullptr;

        const auto& fs = g_App.GetFilesystem();
        if (formatFlags & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.spv", (fs.GetShadersDirectory() / filename).string().c_str());
            formatFlag = SDL_GPU_SHADERFORMAT_SPIRV;
            entryPoint = "main";
        }
        else if (formatFlags & SDL_GPU_SHADERFORMAT_MSL)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.msl", (fs.GetShadersDirectory() / filename).string().c_str());
            formatFlag = SDL_GPU_SHADERFORMAT_MSL;
            entryPoint = "main0";
        }
        else if (formatFlags & SDL_GPU_SHADERFORMAT_DXIL)
        {
            snprintf(fullPath, sizeof(fullPath), "%s.dxil", (fs.GetShadersDirectory() / filename).string().c_str());
            formatFlag = SDL_GPU_SHADERFORMAT_DXIL;
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
            .format               = formatFlag,
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
}

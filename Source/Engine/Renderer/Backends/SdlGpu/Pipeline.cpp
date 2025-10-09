#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/Pipeline.h"

#include "Engine/Services/Filesystem.h"
#include "Engine/Application.h"

using namespace Silent::Services;

namespace Silent::Renderer
{
    struct RendererVertex
    {
        Vector3 Position = Vector3::Zero;
        Color   Col      = Color::Black;
    };

    void PipelineManager::Initialize(SDL_Window& window, SDL_GPUDevice& device)
    {
        _device = &device;

        // Load vertex shader.
        auto* vertShader = LoadShader("RawTriangle.vert", 0, 0, 0, 0);
        if (vertShader == nullptr)
        {
            Log("Failed to create vertex shader.", LogLevel::Error);
        }
        SDL_ReleaseGPUShader(_device, vertShader);

        // Load fragment shader.
        auto* fragShader = LoadShader("SolidColor.frag", 0, 0, 0, 0);
        if (fragShader == nullptr)
        {
            Log("Failed to create fragment shader.", LogLevel::Error);
        }
        SDL_ReleaseGPUShader(_device, fragShader);

        // Load other vertex shader.
        auto* vertShader2 = LoadShader("Vertex.vert", 0, 0, 0, 0);
        if (vertShader2 == nullptr)
        {
            Log("Failed to create vertex shader.", LogLevel::Error);
        }
        SDL_ReleaseGPUShader(_device, vertShader2);

        // Load other fragment shader.
        auto* fragShader2 = LoadShader("Frag.frag", 0, 0, 0, 0);
        if (fragShader2 == nullptr)
        {
            Log("Failed to create fragment shader.", LogLevel::Error);
        }
        SDL_ReleaseGPUShader(_device, fragShader2);

        auto colorTargetDesc = SDL_GPUColorTargetDescription
        {
            .format = SDL_GetGPUSwapchainTextureFormat(_device, &window)
        };
        auto pipelineInfo = SDL_GPUGraphicsPipelineCreateInfo
        {
            .vertex_shader   = vertShader,
            .fragment_shader = fragShader,
            .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .target_info     = SDL_GPUGraphicsPipelineTargetInfo
            {
                .color_target_descriptions = &colorTargetDesc,
                .num_color_targets         = 1
            }
        };

        // Create fill pipeline.
        pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        _pipelines[(int)PipelineType::Fill]           = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
        if (_pipelines[(int)PipelineType::Fill] == nullptr) 
        {
            throw std::runtime_error("Failed to create fill pipeline.");
        }

        // Create line pipeline.
        pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
        _pipelines[(int)PipelineType::Line]           = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
        if (_pipelines[(int)PipelineType::Line] == nullptr)
        {
            throw std::runtime_error("Failed to create line pipeline.");
        }

        // Triangle pipeline. @todo This function is messy. Need a clean way of initializing all pipelines at once.
        // ===============

        auto vertexBufferDesctiptions = std::array<SDL_GPUVertexBufferDescription, 1>{};
        vertexBufferDesctiptions[0].slot               = 0;
        vertexBufferDesctiptions[0].input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertexBufferDesctiptions[0].instance_step_rate = 0;
        vertexBufferDesctiptions[0].pitch              = sizeof(RendererVertex);

        pipelineInfo.rasterizer_state.fill_mode                    = SDL_GPU_FILLMODE_FILL;
        pipelineInfo.vertex_input_state.num_vertex_buffers         = 1;
        pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions.data();

        // describe the vertex attribute
        auto vertexAttributes = std::array<SDL_GPUVertexAttribute, 2>{};

        // a_position
        vertexAttributes[0].buffer_slot = 0;
        vertexAttributes[0].location    = 0;
        vertexAttributes[0].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        vertexAttributes[0].offset      = 0;

        // a_color
        vertexAttributes[1].buffer_slot = 0;
        vertexAttributes[1].location    = 1;
        vertexAttributes[1].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
        vertexAttributes[1].offset      = sizeof(float) * 3;

        pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
        pipelineInfo.vertex_input_state.vertex_attributes     = vertexAttributes.data();

        // describe the color target
        SDL_GPUColorTargetDescription colorTargetDescriptions[1];
        colorTargetDescriptions[0] = {};
        colorTargetDescriptions[0].blend_state.enable_blend          = true;
        colorTargetDescriptions[0].blend_state.color_blend_op        = SDL_GPU_BLENDOP_ADD;
        colorTargetDescriptions[0].blend_state.alpha_blend_op        = SDL_GPU_BLENDOP_ADD;
        colorTargetDescriptions[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        colorTargetDescriptions[0].format                            = SDL_GetGPUSwapchainTextureFormat(_device, &window);

        pipelineInfo.target_info.num_color_targets         = 1;
        pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

        pipelineInfo.vertex_shader   = vertShader2;
        pipelineInfo.fragment_shader = fragShader2;

        // create the pipeline
        _pipelines[(int)PipelineType::Triangle] = SDL_CreateGPUGraphicsPipeline(_device, &pipelineInfo);
        if (_pipelines[(int)PipelineType::Triangle] == nullptr) 
        {
            throw std::runtime_error("Failed to create triangle pipeline.");
        }
    }

    void PipelineManager::Bind(SDL_GPURenderPass& renderPass, PipelineType pipelineType)
    {
        SDL_BindGPUGraphicsPipeline(&renderPass, _pipelines[(int)pipelineType]);
    }

    void PipelineManager::InitializeGraphicsPipeline(const PipelineConfig& config)
    {
        
    }

    SDL_GPUShader* PipelineManager::LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount)
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
            SDL_free(code);
            return nullptr;
        }
        SDL_free(code);

        return shader;
    }
}

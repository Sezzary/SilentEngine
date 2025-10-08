#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/Pipeline.h"

#include "Engine/Services/Filesystem.h"
#include "Engine/Application.h"

using namespace Silent::Services;

namespace Silent::Renderer
{
    void PipelineManager::Initialize(SDL_Window& window, SDL_GPUDevice& device)
    {
        _device = &device;

        // Load vertex shader.
        auto* vertShader = LoadShader("RawTriangle.vert", 0, 0, 0, 0);
        if (vertShader == nullptr)
        {
            Log("Failed to create vertex shader.", LogLevel::Error);
        }

        // Load fragment shader.
        auto* fragShader = LoadShader("SolidColor.frag", 0, 0, 0, 0);
        if (fragShader == nullptr)
        {
            Log("Failed to create fragment shader.", LogLevel::Error);
        }

        auto colorTargetDesc = SDL_GPUColorTargetDescription
        {
            .format = SDL_GetGPUSwapchainTextureFormat(_device, &window)
        };
        auto pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo
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
        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        _pipelines[(int)PipelineType::Fill]           = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_pipelines[(int)PipelineType::Fill] == nullptr) 
        {
            throw std::runtime_error("Failed to create 3D fill pipeline.");
        }

        // Create line pipeline.
        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
        _pipelines[(int)PipelineType::Line]           = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_pipelines[(int)PipelineType::Line] == nullptr)
        {
            throw std::runtime_error("Failed to create 3D line pipeline.");
        }

        // Clean up shader resources.
        SDL_ReleaseGPUShader(_device, vertShader);
        SDL_ReleaseGPUShader(_device, fragShader);
    }

    void PipelineManager::Bind(PipelineType pipelineType, SDL_GPURenderPass& renderPass)
    {
        SDL_BindGPUGraphicsPipeline(&renderPass, _pipelines[(int)pipelineType]);
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

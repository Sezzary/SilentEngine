#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Engine/Application.h"

namespace Silent::Renderer
{
    SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const std::string& shaderFilename,
                              uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount)
    {
        auto stage = SDL_GPUShaderStage{};
        if (SDL_strstr(shaderFilename.c_str(), ".vert"))
        {
            stage = SDL_GPU_SHADERSTAGE_VERTEX;
        }
        else if (SDL_strstr(shaderFilename.c_str(), ".frag"))
        {
            stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        }
        else
        {
            Log("Invalid shader stage.", LogLevel::Error);
            return nullptr;
        }

        char        fullPath[256];
        auto        backendFormats = SDL_GetGPUShaderFormats(device);
        auto        format         = (SDL_GPUShaderFormat)SDL_GPU_SHADERFORMAT_INVALID;
        const char* entryPoint     = nullptr;

        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.spv", SDL_GetBasePath(), shaderFilename.c_str());
            format     = SDL_GPU_SHADERFORMAT_SPIRV;
            entryPoint = "main";
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.msl", SDL_GetBasePath(), shaderFilename.c_str());
            format     = SDL_GPU_SHADERFORMAT_MSL;
            entryPoint = "main0";
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.dxil", SDL_GetBasePath(), shaderFilename.c_str());
            format     = SDL_GPU_SHADERFORMAT_DXIL;
            entryPoint = "main";
        }
        else
        {
            Log("Unrecognized backend shader format.", LogLevel::Error);
            return nullptr;
        }

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
            .format               = format,
            .stage                = stage,
            .num_samplers         = samplerCount,
            .num_storage_textures = storageTexCount,
            .num_storage_buffers  = storageBufferCount,
            .num_uniform_buffers  = uniBufferCount
        };

        auto* shader = SDL_CreateGPUShader(device, &shaderInfo);
        if (shader == nullptr)
        {
            Log("Failed to create shader: " + std::string(SDL_GetError()));
            SDL_free(code);
            return nullptr;
        }

        SDL_free(code);
        return shader;
    }

    static SDL_GPUColorTargetDescription ColorTargetDesc = {};
    void SdlGpuRenderer::Initialize(SDL_Window& window)
    {
        _type   = RendererType::SdlGpu;
        _window = &window;

        Log("Using SDL_gpu renderer.");

        // Collect GPU flags.
        int formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;

        // Create GPU device.
        _device = SDL_CreateGPUDevice(formatFlags, IS_DEBUG_BUILD, nullptr);
        if (_device == nullptr)
        {
            throw std::runtime_error("Failed to create SDL GPU device: " + std::string(SDL_GetError()));
        }

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_device, _window))
        {
            throw std::runtime_error("Failed to claim window for SDL GPU device: " + std::string(SDL_GetError()));
        }

        auto* vertexShader = LoadShader(_device, "RawTriangle.vert", 0, 0, 0, 0);
        if (vertexShader == NULL)
        {
            Log("Failed to create vertex shader.", LogLevel::Error);
        }

        auto* fragmentShader = LoadShader(_device, "SolidColor.frag", 0, 0, 0, 0);
        if (fragmentShader == NULL)
        {
            Log("Failed to create fragment shader.", LogLevel::Error);
        }

        ColorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(_device, _window);

        auto pipelineCreateInfo                                  = SDL_GPUGraphicsPipelineCreateInfo{};
        pipelineCreateInfo.primitive_type                        = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        pipelineCreateInfo.vertex_shader                         = vertexShader;
        pipelineCreateInfo.fragment_shader                       = fragmentShader;
        pipelineCreateInfo.target_info.num_color_targets         = 1;
        pipelineCreateInfo.target_info.color_target_descriptions = &ColorTargetDesc;

        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        _fillPipeline                                 = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_fillPipeline == NULL) 
        {
            throw std::runtime_error("Failed to create fill pipeline.");
        }

        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
        _linePipeline                                 = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_linePipeline == NULL)
        {
            throw std::runtime_error("Failed to create line pipeline.");
        }
        
        // Clean up shader resources
        SDL_ReleaseGPUShader(_device, vertexShader);
        SDL_ReleaseGPUShader(_device, fragmentShader);

        CreateDebugGui();
    }

    void SdlGpuRenderer::Deinitialize()
    {

    }

    static void DrawTriangle()
    {

    }

    static SDL_GPUViewport SmallViewport = { 160, 120, 320, 240, 0.1f, 1.0f };
    static SDL_Rect        ScissorRect   = { 320, 240, 320, 240 };
    void SdlGpuRenderer::Update()
    {
        // Acquire command buffer.
        auto* cmdBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (cmdBuffer == nullptr)
        {
            throw std::runtime_error("Failed to acquire SDL GPU command buffer: " + std::string(SDL_GetError()));
        }

        // Acquire GPU swapchain texture.
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, _window, &_swapchainTexture, NULL, NULL))
        {
            throw std::runtime_error("Failed to acquire SDL GPU swapchain texture: " + std::string(SDL_GetError()));
        }

        if (_swapchainTexture != nullptr)
        {
            auto colorTargetInfo        = SDL_GPUColorTargetInfo{};
            colorTargetInfo.texture     = _swapchainTexture;
            colorTargetInfo.clear_color = SDL_FColor{ _clearColor.R(), _clearColor.G(), _clearColor.B(), 0.0f };
            colorTargetInfo.load_op     = SDL_GPU_LOADOP_CLEAR;
            colorTargetInfo.store_op    = SDL_GPU_STOREOP_STORE;

            auto* renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1, NULL);
            SDL_BindGPUGraphicsPipeline(renderPass, _fillPipeline);
            SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
            SDL_EndGPURenderPass(renderPass);
            /*SDL_BindGPUGraphicsPipeline(renderPass, _fillPipeline);
            SDL_SetGPUViewport(renderPass, &SmallViewport);
            SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
            SDL_EndGPURenderPass(renderPass);*/
        }

        SDL_SubmitGPUCommandBuffer(cmdBuffer);
    }

    void SdlGpuRenderer::RefreshTextureFilter()
    {

    }

    void SdlGpuRenderer::SaveScreenshot() const
    {

    }

    void SdlGpuRenderer::LogError(const std::string& msg) const
    {

    }

    void SdlGpuRenderer::SubmitPrimitive2d(const Primitive2d& prim)
    {

    }

    void SdlGpuRenderer::SubmitScreenSprite(int assetIdx, const Vector2& pos, short rot, const Vector2& scale, const Color& color,
                                            int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode)
    {

    }

    void SdlGpuRenderer::DrawDebugGui()
    {
        // @todo Not working. Needs extra params.

        // If debug GUI is disabled, return early.
        /*const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            _debugGuiDrawCalls.clear();
            return;
        }

        auto res = GetScreenResolution();
        SDL_GetWindowSize(_window, &res.x, &res.y);
        ImGui::GetIO().DisplaySize = ImVec2((float)res.x, (float)res.y);

        ImGui_ImplSDLGPU3_NewFrame();
        ImGui::NewFrame();

        // Draw GUIs.
        for (auto& drawFunc : _debugGuiDrawCalls)
        {
            drawFunc();
        }
        _debugGuiDrawCalls.clear();

        ImGui::Render();
        ImGui_ImplSDLGPU3_PrepareDrawData(ImGui::GetDrawData(), cmdBuffer);
        ImGui_ImplSDLGPU3_RenderDrawData(ImGui::GetDrawData(), cmdBuffer, renderPass);*/
    }

    void SdlGpuRenderer::CreateDebugGui()
    {
        auto initInfo = ImGui_ImplSDLGPU3_InitInfo
        {
            .Device            = _device,
            .ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(_device, _window),
            .MSAASamples       = SDL_GPU_SAMPLECOUNT_1
        };

        ImGui_ImplSDL3_InitForSDLGPU(_window);
        ImGui_ImplSDLGPU3_Init(&initInfo);
    }
}

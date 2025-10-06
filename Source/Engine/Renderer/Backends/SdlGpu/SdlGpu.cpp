#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Engine/Application.h"

namespace Silent::Renderer
{
    struct RendererVertex
    {
        float x, y, z;
        float r, g, b, a;
    };

    static const auto VERTICES = std::vector<RendererVertex>
    {
        {  0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }
    };

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
            throw std::runtime_error("Failed to create GPU device: " + std::string(SDL_GetError()));
        }

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_device, _window))
        {
            throw std::runtime_error("Failed to claim window for GPU device: " + std::string(SDL_GetError()));
        }

        auto* vertShader = LoadShader("RawTriangle.vert", 0, 0, 0, 0);
        if (vertShader == nullptr)
        {
            Log("Failed to create vertex shader.", LogLevel::Error);
        }

        auto* fragShader = LoadShader("SolidColor.frag", 0, 0, 0, 0);
        if (fragShader == nullptr)
        {
            Log("Failed to create fragment shader.", LogLevel::Error);
        }

        auto colorTargetDesc = SDL_GPUColorTargetDescription
        {
            .format = SDL_GetGPUSwapchainTextureFormat(_device, _window)
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

        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        _fillPipeline3d                               = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_fillPipeline3d == nullptr) 
        {
            throw std::runtime_error("Failed to create 3D fill pipeline.");
        }

        pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
        _linePipeline3d                               = SDL_CreateGPUGraphicsPipeline(_device, &pipelineCreateInfo);
        if (_linePipeline3d == nullptr)
        {
            throw std::runtime_error("Failed to create 3D line pipeline.");
        }

        // TODO: 2D pipelines.

        // Clean up shader resources.
        SDL_ReleaseGPUShader(_device, vertShader);
        SDL_ReleaseGPUShader(_device, fragShader);

        /*auto bufferInfo = SDL_GPUBufferCreateInfo
        {
            .size  = sizeof(VERTICES),
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX
        };
        _vertexBuffer = SDL_CreateGPUBuffer(_device, &bufferInfo);

        auto transferInfo = SDL_GPUTransferBufferCreateInfo
        {
            .size  = sizeof(VERTICES),
            .usage = SDL_GPU_TRANSFER_BUFFER_USAGE_UPLOAD
        };
        _transferBuffer = SDL_CreateGPUTransferBuffer(_device, &transferInfo);

        auto* data = (RendererVertex*)SDL_MapGPUTransferBuffer(_device, _transferBuffer, false);
        data[0] = VERTICES[0];
        data[1] = VERTICES[1];
        data[2] = VERTICES[2];*/

        // Create ImGui context.
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplSDL3_InitForSDLGPU(_window);

        // Initialize ImGui backend.
        auto initInfo = ImGui_ImplSDLGPU3_InitInfo
        {
            .Device            = _device,
            .ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(_device, _window),
            .MSAASamples       = SDL_GPU_SAMPLECOUNT_1
        };
        ImGui_ImplSDLGPU3_Init(&initInfo);

        // Reserve memory.
        _primitives2d.reserve(PRIMITIVE_2D_COUNT_MAX);
    }

    // @todo Has errors.
    void SdlGpuRenderer::Deinitialize()
    {
        SDL_WaitForGPUIdle(_device);

        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();

        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyGPUDevice(_device);
    }

    void SdlGpuRenderer::Update()
    {
        // Reset.
        _drawCallCount = 0;

        // Acquire command buffer.
        _commandBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (_commandBuffer == nullptr)
        {
            Log("Failed to acquire command buffer: " + std::string(SDL_GetError()), LogLevel::Error);
            return;
        }

        // Acquire swapchain texture.
        _swapchainTexture = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(_commandBuffer, _window, &_swapchainTexture, nullptr, nullptr))
        {
            Log("Failed to acquire swapchain texture: " + std::string(SDL_GetError()), LogLevel::Error);
            return;
        }

        // Draw frame.
        if (_swapchainTexture != nullptr)
        {
            Draw3dScene();
            //Draw2dScene();
            DrawDebugGui();
        }

        // Submit command buffer.
        SDL_SubmitGPUCommandBuffer(_commandBuffer);

        // Clear.
        _primitives2d.clear();
    }

    void SdlGpuRenderer::RefreshTextureFilter()
    {
        // @todo
    }

    void SdlGpuRenderer::SaveScreenshot() const
    {
        constexpr uint COLOR_CHANNEL_COUNT = 3; // RGB.

        const auto& fs = g_App.GetFilesystem();

        // Get window size.
        auto res = GetScreenResolution();

        // Ensure directory exists.
        auto timestamp = GetCurrentDateString() + "_" + GetCurrentTimeString();
        auto filename  = (SCREENSHOT_FILENAME_BASE + timestamp) + PNG_FILE_EXT;
        auto path      = fs.GetScreenshotsFolder() / filename;
        std::filesystem::create_directories(path.parent_path());

        // Get window surface.
        auto* surface = SDL_GetWindowSurface(_window);
        if (surface == nullptr)
        {
            Log("Failed to capture screenshot: " + std::string(SDL_GetError()), LogLevel::Error, LogMode::DebugRelease, true);
            return;
        }

        // Lock surface to access pixels.
        if (SDL_LockSurface(surface))
        {
            Log("Failed to capture screenshot: " + std::string(SDL_GetError()), LogLevel::Error, LogMode::DebugRelease, true);
            return;
        }

        // Write screenshot file.
        if (!stbi_write_png(path.string().c_str(), res.x, res.y, COLOR_CHANNEL_COUNT, surface->pixels, res.x * COLOR_CHANNEL_COUNT))
        {
            Log("Failed to save screenshot.", LogLevel::Error, LogMode::DebugRelease, true);
        }

        SDL_UnlockSurface(surface);
    }

    void SdlGpuRenderer::LogError(const std::string& msg) const
    {
        // Not needed?
    }

    void SdlGpuRenderer::SubmitPrimitive2d(const Primitive2d& prim)
    {
        if (_primitives2d.size() >= PRIMITIVE_2D_COUNT_MAX)
        {
            Log("Attampted to add 2D primitive to full container.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        _primitives2d.push_back(prim);
    }

    void SdlGpuRenderer::SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                            const Color& color, int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode)
    {
        // @todo
    }

    void SdlGpuRenderer::Draw3dScene()
    {
        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture     = _swapchainTexture,
            .clear_color = SDL_FColor{ _clearColor.R(), _clearColor.G(), _clearColor.B(), _clearColor.A() },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        SDL_BindGPUGraphicsPipeline(renderPass, g_DebugData.EnableWireframeMode ? _linePipeline3d : _fillPipeline3d);
        SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(renderPass);
    }

    void SdlGpuRenderer::Draw2dScene()
    {
        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTexture,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        SDL_BindGPUGraphicsPipeline(renderPass, g_DebugData.EnableWireframeMode ? _linePipeline2d : _fillPipeline2d);
        SDL_DrawGPUPrimitives(renderPass, 4, 1, 0, 0);
        SDL_EndGPURenderPass(renderPass);
    }

    void SdlGpuRenderer::DrawDebugGui()
    {
        // If debug GUI is disabled, return early.
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            _debugGuiDrawCalls.clear();
            return;
        }

        // Start new frame.
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Draw GUIs.
        for (auto& drawCall : _debugGuiDrawCalls)
        {
            drawCall();
        }
        _debugGuiDrawCalls.clear();

        // Prepare render data.
        ImGui::Render();
        auto* drawData = ImGui::GetDrawData();
        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, _commandBuffer);

        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTexture,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        ImGui_ImplSDLGPU3_RenderDrawData(drawData, _commandBuffer, renderPass);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_GPUShader* SdlGpuRenderer::LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount)
    {
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

        char        fullPath[256];
        auto        backendFormats = SDL_GetGPUShaderFormats(_device);
        auto        format         = (SDL_GPUShaderFormat)SDL_GPU_SHADERFORMAT_INVALID;
        const char* entryPoint     = nullptr;

        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.spv", SDL_GetBasePath(), (std::string(SHADERS_FOLDER_NAME) + "/" + filename).c_str());
            format     = SDL_GPU_SHADERFORMAT_SPIRV;
            entryPoint = "main";
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.msl", SDL_GetBasePath(), (std::string(SHADERS_FOLDER_NAME) + "/" + filename).c_str());
            format     = SDL_GPU_SHADERFORMAT_MSL;
            entryPoint = "main0";
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s%s.dxil", SDL_GetBasePath(), (std::string(SHADERS_FOLDER_NAME) + "/" + filename).c_str());
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

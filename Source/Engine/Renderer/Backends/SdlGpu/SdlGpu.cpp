#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Backends/SdlGpu/Pipeline.h"
#include "Engine/Services/Filesystem.h"

using namespace Silent::Services;

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
        Log("Using SDL_gpu renderer.");

        _type   = RendererType::SdlGpu;
        _window = &window;

        // Create GPU device.
        int formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
        _device         = SDL_CreateGPUDevice(formatFlags, IS_DEBUG_BUILD, nullptr);
        if (_device == nullptr)
        {
            throw std::runtime_error("Failed to create GPU device: " + std::string(SDL_GetError()));
        }

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_device, _window))
        {
            throw std::runtime_error("Failed to claim window for GPU device: " + std::string(SDL_GetError()));
        }

        // Initialize pipelines with shaders.
        _pipelines.Initialize(*_window, *_device);

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
        auto path      = fs.GetScreenshotsDirectory() / filename;
        std::filesystem::create_directories(path.parent_path());

        // Get window surface.
        auto* surface = SDL_GetWindowSurface(_window);
        if (surface == nullptr)
        {
            Log("Failed to save screenshot: " + std::string(SDL_GetError()), LogLevel::Warning, LogMode::DebugRelease, true);
            return;
        }

        // Lock surface to access pixels.
        if (!SDL_LockSurface(surface))
        {
            Log("Failed to save screenshot: " + std::string(SDL_GetError()), LogLevel::Warning, LogMode::DebugRelease, true);
            return;
        }

        // Write screenshot file.
        if (stbi_write_png(path.string().c_str(), res.x, res.y, COLOR_CHANNEL_COUNT, surface->pixels, res.x * COLOR_CHANNEL_COUNT))
        {
            Log("Saved screenshot.", LogLevel::Info, LogMode::DebugRelease, true);
        }
        else
        {
            Log("Failed to save screenshot.", LogLevel::Warning, LogMode::DebugRelease, true);
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
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        _pipelines.Bind(g_DebugData.EnableWireframeMode ? PipelineType::Line : PipelineType::Fill, renderPass);
        SDL_DrawGPUPrimitives(&renderPass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(&renderPass);
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
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        _pipelines.Bind(g_DebugData.EnableWireframeMode ? PipelineType::Line : PipelineType::Fill, renderPass);
        SDL_DrawGPUPrimitives(&renderPass, 4, 1, 0, 0);
        SDL_EndGPURenderPass(&renderPass);
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
}

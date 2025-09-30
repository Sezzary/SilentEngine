#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Engine/Application.h"

namespace Silent::Renderer
{
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

        CreateDebugGui();
    }

    void SdlGpuRenderer::Deinitialize()
    {

    }

    void SdlGpuRenderer::Update()
    {
        // Asquire command buffer.
        auto* cmdBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (cmdBuffer == nullptr)
        {
            throw std::runtime_error("Failed to acquire SDL GPU command buffer: " + std::string(SDL_GetError()));
        }

        // Wait and acquire GPU swapchain texture.
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, _window, &_swapchainTexture, NULL, NULL))
        {
            throw std::runtime_error("Failed to acquire SDL GPU swapchain texture: " + std::string(SDL_GetError()));
        }

        auto colorTargetInfo        = SDL_GPUColorTargetInfo{};
        colorTargetInfo.texture     = _swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{ _clearColor.R(), _clearColor.G(), _clearColor.B(), _clearColor.A() };
        colorTargetInfo.load_op     = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op    = SDL_GPU_STOREOP_STORE;

        auto* renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1, NULL);

        SDL_EndGPURenderPass(renderPass);
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

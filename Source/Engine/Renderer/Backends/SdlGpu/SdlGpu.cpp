#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

namespace Silent::Renderer
{
    void SdlGpuRenderer::Initialize(SDL_Window& window)
    {
        _type   = RendererType::SdlGpu;
        _window = &window;

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

        // Asquire command buffer.
        _cmdBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (_cmdBuffer == nullptr)
        {
            throw std::runtime_error("Failed to acquire SDL GPU command buffer: " + std::string(SDL_GetError()));
        }

        // Wait and acquire GPU swapchain texture.
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(_cmdBuffer, _window, &_swapchainTexture, NULL, NULL))
        {
            throw std::runtime_error("Failed to wait and acquire SDL GPU swapchain texture: " + std::string(SDL_GetError()));
        }
    }
    
    void SdlGpuRenderer::Deinitialize()
    {

    }

    void SdlGpuRenderer::Update()
    {
        auto colorTargetInfo        = SDL_GPUColorTargetInfo{};
        colorTargetInfo.texture     = _swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{ 0.3f, 0.6f, 0.5f, 1.0f };
        colorTargetInfo.load_op     = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op    = SDL_GPU_STOREOP_STORE;

        auto* renderPass = SDL_BeginGPURenderPass(_cmdBuffer, &colorTargetInfo, 1, NULL);
        SDL_EndGPURenderPass(renderPass);

        SDL_SubmitGPUCommandBuffer(_cmdBuffer);
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
}

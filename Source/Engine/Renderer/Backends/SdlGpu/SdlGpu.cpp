#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

namespace Silent::Renderer
{
    void SdlGpuRenderer::Initialize(SDL_Window& window)
    {
        _type   = RendererType::SdlGpu;
        _window = &window;

        _gpuDevice = SDL_CreateGPUDevice(0, IS_DEBUG_BUILD, nullptr);
        if (_gpuDevice == nullptr)
        {
            throw std::runtime_error("Failed to create SDL GPU device.");
        }

        SDL_ClaimWindowForGPUDevice(_gpuDevice, _window);

        // Set the viewport
        auto res = GetScreenResolution();
        //SDL_GPU_SetViewport(_gpuDevice, 0, 0, res.x, res.y);

        // Clear screen to black
        //SDL_GPU_Clear(_gpuDevice);

        // TODO: Initialize shaders, buffers, and other resources
    }
    
    void SdlGpuRenderer::Deinitialize()
    {

    }

    void SdlGpuRenderer::Update()
    {

    }

    void SdlGpuRenderer::RefreshTextureFilter()
    {

    }

    void SdlGpuRenderer::SaveScreenshot() const
    {

    }
}

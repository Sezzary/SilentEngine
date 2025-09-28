#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"

namespace Silent::Renderer
{
    void SdlGpuRenderer::Initialize(SDL_Window& window)
    {
        _type   = RendererType::SdlGpu;
        _window = &window;

        // Collect GPU flags.
        int gpuFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;

        // Create GPU device.
        _gpuDevice = SDL_CreateGPUDevice(gpuFlags, IS_DEBUG_BUILD, nullptr);
        if (_gpuDevice == nullptr)
        {
            throw std::runtime_error("Failed to create SDL GPU device: " + std::string(SDL_GetError()));
        }

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_gpuDevice, _window))
        {
            throw std::runtime_error("Failed to claim window for SDL GPU device: " + std::string(SDL_GetError()));
        }

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

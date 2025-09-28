#pragma once

#include "Engine/Renderer/Base.h"

namespace Silent::Renderer
{
    class SdlGpuRenderer : public RendererBase
    {
    private:
        // Fields

        SDL_GPUDevice* _gpuDevice = nullptr;

    public:
        // Constructors

        SdlGpuRenderer() = default;

        // Utilities

        void Initialize(SDL_Window& window) override;
        void Deinitialize() override;
        void Update() override;
        void RefreshTextureFilter() override;
        void SaveScreenshot() const override;

    private:
        // Helpers

        
    };
}

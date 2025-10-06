#pragma once

#include "Engine/Renderer/Renderer.h"

namespace Silent::Renderer
{
    class SdlGpuRenderer : public RendererBase
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*           _device           = nullptr;
        SDL_GPUTexture*          _swapchainTexture = nullptr;
        SDL_GPUCommandBuffer*    _commandBuffer    = nullptr;
        SDL_GPUBuffer*           _vertexBuffer     = nullptr;
        SDL_GPUTransferBuffer*   _transferBuffer   = nullptr;
        SDL_GPUGraphicsPipeline* _fillPipeline3d   = nullptr;
        SDL_GPUGraphicsPipeline* _fillPipeline2d   = nullptr;
        SDL_GPUGraphicsPipeline* _linePipeline3d   = nullptr;
        SDL_GPUGraphicsPipeline* _linePipeline2d   = nullptr;

    public:
        // =============
        // Constructors
        // =============

        SdlGpuRenderer() = default;

        // ==========
        // Utilities
        // ==========

        void Initialize(SDL_Window& window) override;
        void Deinitialize() override;
        void Update() override;
        void RefreshTextureFilter() override;
        void SaveScreenshot() const override;
        void LogError(const std::string& msg) const override;

        void SubmitPrimitive2d(const Primitive2d& prim) override;
        void SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                const Color& color, int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode) override;

    private:
        // ========
        // Helpers
        // ========

        void Draw3dScene() override;
        void Draw2dScene() override;
        void DrawDebugGui() override;
    };
}

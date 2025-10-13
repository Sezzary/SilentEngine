#pragma once

#include "Engine/Renderer/Backends/SdlGpu/Buffer.h"
#include "Engine/Renderer/Backends/SdlGpu/Pipeline.h"
#include "Engine/Renderer/Common/Objects/Vertex2d.h"
#include "Engine/Renderer/Common/Objects/Vertex3d.h"
#include "Engine/Renderer/Common/View.h"
#include "Engine/Renderer/Renderer.h"

namespace Silent::Renderer
{
    struct BufferVertex
    {
        Vector3 Position = Vector3::Zero;
        Color   Col      = Color::Black;
    };
    struct BufferTexVertex
    {
        Vector3 Position = Vector3::Zero;
        Vector2 Uv       = Vector2::Zero;
    };

    struct BufferData
    {
        Buffer<BufferVertex>    Primitives2d = Buffer<BufferVertex>();
        Buffer<BufferTexVertex> TexQuad      = Buffer<BufferTexVertex>();
        Buffer<uint16>          TexQuadIdxs  = Buffer<uint16>();

        Buffer<Vertex2d>     Sprites2d    = Buffer<Vertex2d>();
    };

    class SdlGpuRenderer : public RendererBase
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*               _device           = nullptr;           /** GPU device. */
        SDL_GPUTexture*              _swapchainTexture = nullptr;           /** Active swapchain texture. */
        SDL_GPUCommandBuffer*        _commandBuffer    = nullptr;           /** Active command buffer. */
        std::vector<SDL_GPUSampler*> _samplers         = {};                /** Texture samplers. */
        BufferData                   _buffers          = {};                /** Vertex, index, and indirect buffers. */
        PipelineManager              _pipelines        = PipelineManager(); /** Pipeline handler. */
        View                         _view             = View();            /** Camera view. */

        std::unordered_map<SDL_GPUTexture*, int> _textureCache = {}; /** Key = texture, value = asset ID. */

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `SdlGpuRenderer`. */
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

        void Submit2dPrimitive(const Primitive2d& prim) override;
        void SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                const Color& color, int depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode) override;

    private:
        // ========
        // Helpers
        // ========

        void Draw3dScene() override;
        void Draw2dScene() override;
        void DrawDebugGui() override;

        void Copy2dPrimitives(SDL_GPUCopyPass& copyPass, std::vector<BufferVertex>& bufferVerts);
    };
}

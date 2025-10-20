#pragma once

#include "Renderer/Backends/SdlGpu/Buffer.h"
#include "Renderer/Backends/SdlGpu/Pipeline.h"
#include "Renderer/Common/Objects/Primitive/Vertex2d.h"
#include "Renderer/Common/Objects/Primitive/Vertex3d.h"
#include "Renderer/Renderer.h"

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
        Buffer<BufferVertex> Primitives2d = Buffer<BufferVertex>();
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

        std::unordered_map<int, SDL_GPUTexture*> _textureCache = {}; /** Key = asset index, value = texture. */

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
        void SaveScreenshot() const override;

    private:
        // ========
        // Helpers
        // ========

        void Draw3dScene() override;
        void Draw2dScene() override;
        void DrawPostProcess() override;
        void DrawDebugGui() override;

        void Copy2dPrimitives(SDL_GPUCopyPass& copyPass, std::vector<BufferVertex>& bufferVerts);
    };
}

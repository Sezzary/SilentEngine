#include "Framework.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Application.h"
#include "Assets/Fonts.h"
#include "Renderer/Backends/SdlGpu/Resources/TextureCache.h"
#include "Renderer/Common/Resources/Layouts/Buffers.h"
#include "Renderer/Common/Resources/Layouts/Uniforms.h"
#include "Renderer/Common/Resources/Primitive/Vertex2d.h"
#include "Renderer/Common/Resources/Primitive/Vertex3d.h"
#include "Renderer/Renderer.h"
#include "Utils/Font.h"
#include "Utils/Utils.h"
#include "Utils/Video.h"

using namespace Silent::Utils;

namespace Silent::Renderer::SdlGpu
{
    void Renderer::InitializeGpuBuffers()
    {
        constexpr int SPRITE_2D_VERT_COUNT_MAX = SPRITE_2D_COUNT_MAX * QUAD_VERTEX_COUNT;
        constexpr int SPRITE_2D_IDX_COUNT_MAX  = SPRITE_2D_COUNT_MAX * QUAD_IDX_COUNT;
        constexpr int SHAPE_2D_VERT_COUNT_MAX  = (SHAPE_2D_COUNT_MAX * 2) * TRI_VERTEX_COUNT;
        constexpr int SHAPE_2D_IDX_COUNT_MAX   = SHAPE_2D_VERT_COUNT_MAX;
        constexpr int GLYPH_2D_VERT_COUNT_MAX  = GLYPH_2D_COUNT_MAX * QUAD_VERTEX_COUNT;
        constexpr int GLYPH_2D_IDX_COUNT_MAX   = GLYPH_2D_COUNT_MAX * QUAD_IDX_COUNT;
        constexpr int TRI_3D_VERT_COUNT_MAX    = TRI_3D_COUNT_MAX * TRI_VERTEX_COUNT;
        constexpr int TRI_3D_IDX_COUNT_MAX     = TRI_3D_COUNT_MAX * TRI_IDX_COUNT;

        constexpr int VERT_2D_COUNT_MAX       = SPRITE_2D_VERT_COUNT_MAX +
                                                SHAPE_2D_VERT_COUNT_MAX +
                                                GLYPH_2D_VERT_COUNT_MAX;
        constexpr int VERT_2D_IDX_COUNT_MAX   = SPRITE_2D_IDX_COUNT_MAX +
                                                SHAPE_2D_IDX_COUNT_MAX +
                                                GLYPH_2D_IDX_COUNT_MAX;
        constexpr int PRIM_2D_BATCH_COUNT_MAX = SPRITE_2D_COUNT_MAX +
                                                SHAPE_2D_COUNT_MAX +
                                                GLYPH_2D_COUNT_MAX;
        constexpr int VERT_3D_COUNT_MAX       = TRI_3D_VERT_COUNT_MAX;
        constexpr int VERT_3D_IDX_COUNT_MAX   = VERT_3D_COUNT_MAX;
        constexpr int PRIM_3D_BATCH_COUNT_MAX = (VERT_3D_IDX_COUNT_MAX / TRI_VERTEX_COUNT) * 2; // @todo Check, because immediate + models.

        // Initialize GPU buffers.
        _gpuBuffers.ViewportVertices.Initialize(*_device, QUAD_VERTEX_COUNT, QUAD_IDX_COUNT, "2D viewport vertices");
        _gpuBuffers.ImmediateVertices2d.Initialize(*_device, VERT_2D_COUNT_MAX, VERT_2D_IDX_COUNT_MAX, "Immediate 2D vertices");
        _gpuBuffers.ImmediateVertices3d.Initialize(*_device, VERT_3D_COUNT_MAX, VERT_3D_IDX_COUNT_MAX, "Immediate 3D vertices");
        _meshes = std::make_unique<MeshCache>(*_device, VERT_3D_COUNT_MAX, VERT_3D_IDX_COUNT_MAX, "3D meshes vertices");

        // Reserve draw batches.
        _drawBatches.Primitives2d.reserve(PRIM_2D_BATCH_COUNT_MAX);
        _drawBatches.Primitives3d.reserve(PRIM_3D_BATCH_COUNT_MAX);
    }

    void Renderer::UpdateResources(SDL_GPUCopyPass& copyPass)
    {
        constexpr auto FONT_ATLAS_RES = Vector2i(Font::ATLAS_SIZE);

        auto& fonts = g_App.GetFonts();
        auto& video = g_App.GetVideo();

        // Release/upload textures.
        auto& texs = GetTextures();
        for (const auto& name : _scene.Frame.Front.TextureReleaseQueue)
        {
            texs.Release(name);
        }
        for (const auto& assetName : _scene.Frame.Front.TextureUploadQueue)
        {
            texs.Upload(copyPass, assetName);
        }

        // Release/upload meshes.
        auto& meshes = GetMeshes();
        for (const auto& name : _scene.Frame.Front.MeshReleaseQueue)
        {
            meshes.Release(name);
        }
        for (const auto& assetName : _scene.Frame.Front.MeshUploadQueue)
        {
            meshes.Upload(copyPass, assetName);
        }

        // Run through registered fonts.
        for (const auto& metadata : FONTS_METADATA)
        {
            // Get font.
            auto* font = fonts.GetFont(metadata.Name);
            if (font == nullptr)
            {
                continue;
            }

            // Run through texture atlases.
            const auto& atlases = font->GetTextureAtlases();
            for (int atlasIdx : atlases.UpdatedIdxs)
            {
                const auto& atlas = atlases.Textures[atlasIdx];

                // Upload/update font atlas texture.
                auto  name = metadata.Name + std::to_string(atlasIdx);
                auto* tex  = GetTextures()[name];
                if (tex != nullptr)
                {
                    tex->Update(copyPass, ToSpan(atlas), Vector2i::Zero, FONT_ATLAS_RES);
                }
                else
                {
                    GetTextures().Upload(copyPass, ToSpan(atlas), FONT_ATLAS_RES, name);
                }
            }
        }

        // Upload/update video texture.
        const auto& videoName = video.GetName();
        if (video.IsPlaying())
        {
            auto* tex = GetTextures()[videoName];
            if (tex != nullptr)
            {
                tex->Update(copyPass, ToSpan(video.GetVideoFrame()), Vector2i::Zero, video.GetResolution());
            }
            else
            {
                GetTextures().Upload(copyPass, ToSpan(video.GetVideoFrame()), video.GetResolution(), video.GetName());
            }
        }
    }

    void Renderer::CopyImmediatePrimitives2d(SDL_GPUCopyPass& copyPass)
    {
        auto bufferVerts = std::vector<BufferVertex2d>{};
        auto bufferIdxs  = std::vector<uint16>{};

        // Reserve memory.
        bufferVerts.reserve(_scene.Frame.Front.ImmediatePrimitives2d.size() * QUAD_VERTEX_COUNT);
        bufferIdxs.reserve(_scene.Frame.Front.ImmediatePrimitives2d.size() * QUAD_IDX_COUNT);

        // Create batched GPU buffer data.
        int vertOffset = 0;
        int idxOffset  = 0;
        for (const auto& prim : _scene.Frame.Front.ImmediatePrimitives2d)
        {
            // Add vertices.
            for (int i = 0; i < prim.Vertices.size(); i++)
            {
                float depthZ = std::clamp((float)prim.Depth / (float)DEPTH_2D_MAX, 0.0f, 1.0f);
                auto  pos    = Vector3(prim.Vertices[i].Position.x, prim.Vertices[i].Position.y, depthZ);
                bufferVerts.push_back(BufferVertex2d
                {
                    .Position   = pos,
                    .Uv         = prim.Vertices[i].Uv,
                    .Col        = prim.Vertices[i].Col,
                    .PaletteIdx = prim.Vertices[i].PaletteIdx
                });
            }

            int curVertCount = 0;
            int curIdxCount  = 0;

            // Triangle.
            if (prim.Vertices.size() == TRI_VERTEX_COUNT)
            {
                // Add indices.
                for (int i = 0; i < TRI_IDX_COUNT; i++)
                {
                    bufferIdxs.push_back(i);
                }

                // Set buffer region.
                curVertCount = TRI_VERTEX_COUNT;
                curIdxCount  = TRI_IDX_COUNT;
            }
            // Quad.
            else if (prim.Vertices.size() == QUAD_VERTEX_COUNT)
            {
                // Add indices.
                for (int i : QUAD_TRI_IDXS)
                {
                    bufferIdxs.push_back(i);
                }

                // Set buffer region.
                curVertCount = QUAD_VERTEX_COUNT;
                curIdxCount  = QUAD_IDX_COUNT;
            }

            // Add batch.
            // @todo Smarter way that strings together primitives with the same render stage, blend mode, and texture.
            // What to do with uniforms? For now, collect each as its own batch of 2 triangles.
            _drawBatches.Primitives2d.push_back(DrawBatch
            {
                .TextureName  = prim.TextureName,
                .RenderStg    = prim.RenderStg,
                .BlendMd      = prim.BlendMd,
                .Uniform      = prim.Uniform,
                .VertexCount  = curIdxCount,
                .VertexOffset = vertOffset,
                .IdxOffset    = idxOffset
            });

            vertOffset += curVertCount;
            idxOffset  += curIdxCount;
        }

        // Update GPU buffer.
        if (!bufferVerts.empty() && !bufferIdxs.empty())
        {
            _gpuBuffers.ImmediateVertices2d.UpdateVertices(copyPass, ToSpan(bufferVerts), 0);
            _gpuBuffers.ImmediateVertices2d.UpdateIdxs(copyPass, ToSpan(bufferIdxs), 0);
        }
    }

    void Renderer::CopyImmediatePrimitives3d(SDL_GPUCopyPass& copyPass)
    {
        auto bufferVerts = std::vector<BufferVertex3d>{};
        auto bufferIdxs  = std::vector<uint16>{};

        // Reserve memory.
        bufferVerts.reserve(_scene.Frame.Front.ImmediatePrimitives3d.size() * TRI_VERTEX_COUNT);
        bufferIdxs.reserve(_scene.Frame.Front.ImmediatePrimitives3d.size() * TRI_IDX_COUNT);

        // Create batched GPU buffer data.
        int vertOffset = 0;
        int idxOffset  = 0;
        for (const auto& prim : _scene.Frame.Front.ImmediatePrimitives3d)
        {
            // Add vertices.
            for (int i = 0; i < prim.Vertices.size(); i++)
            {
                bufferVerts.push_back(BufferVertex3d
                {
                    .Position = prim.Vertices[i].Position,
                    .Normal   = prim.Vertices[i].Normal,
                    .Uv       = prim.Vertices[i].Uv,
                    .Col      = prim.Vertices[i].Col
                });
            }

            int curVertCount = 0;
            int curIdxCount  = 0;

            // @todo
            // Line.
            /*if (prim.Vertices.size() == LINE_VERTEX_COUNT)
            {
                // Add indices.
                for (int i = 0; i < LINE_IDX_COUNT; i++)
                {
                    bufferIdxs.push_back(i);
                }

                // Set buffer region.
                curVertCount = LINE_VERTEX_COUNT;
                curIdxCount  = LINE_IDX_COUNT;
            }
            // Triangle.
            else */if (prim.Vertices.size() == TRI_VERTEX_COUNT)
            {
                // Add indices.
                for (int i = 0; i < TRI_IDX_COUNT; i++)
                {
                    bufferIdxs.push_back(i);
                }

                // Set buffer region.
                curVertCount = TRI_VERTEX_COUNT;
                curIdxCount  = TRI_IDX_COUNT;
            }
            // Quad.
            else if (prim.Vertices.size() == QUAD_VERTEX_COUNT)
            {
                // Add indices.
                for (int i : QUAD_TRI_IDXS)
                {
                    bufferIdxs.push_back(i);
                }

                // Set buffer region.
                curVertCount = QUAD_VERTEX_COUNT;
                curIdxCount  = QUAD_IDX_COUNT;
            }

            // Add batch.
            // @todo Smarter way that strings together primitives with the same render stage, blend mode, and texture.
            // What to do with uniforms? For now, collect each as its own batch of 2 triangles.
            _drawBatches.Primitives3d.push_back(DrawBatch
            {
                .TextureName  = prim.TextureName,
                .RenderStg    = prim.RenderStg,
                .BlendMd      = prim.BlendMd,
                .Uniform      = prim.Uniform,
                .VertexCount  = curIdxCount,
                .VertexOffset = vertOffset,
                .IdxOffset    = idxOffset
            });

            vertOffset += curVertCount;
            idxOffset  += curIdxCount;
        }

        // Update GPU buffer.
        if (!bufferVerts.empty() && !bufferIdxs.empty())
        {
            _gpuBuffers.ImmediateVertices3d.UpdateVertices(copyPass, ToSpan(bufferVerts), 0);
            _gpuBuffers.ImmediateVertices3d.UpdateIdxs(copyPass, ToSpan(bufferIdxs), 0);
        }
    }

    void Renderer::CopyGpuViewportQuad(SDL_GPUCopyPass& copyPass)
    {
        constexpr auto BUFFER_IDXS = std::array<uint16, QUAD_IDX_COUNT>{ 0, 2, 1, 1, 2, 3 };

        auto  windowRes      = _scene.Frame.Back.SwapchainResolution.ToVector2();
        float windowAspect   = windowRes.x / windowRes.y;
        auto  viewportRes    = GetViewportResolution().ToVector2();
        float viewportAspect = GetViewportAspectRatio();

        auto scale = Vector2::One;//viewportRes / windowRes;

        auto bufferVerts = std::vector<BufferVertex2d>
        {
            {
                .Position = Vector3(-scale.x, scale.y, 0.0f),
                .Uv       = Vector2(0.0f, 0.0f),
                .Col      = Color::White
            },
            {
                .Position = Vector3(scale.x, scale.y, 0.0f),
                .Uv       = Vector2(1.0f, 0.0f),
                .Col      = Color::White
            },
            {
                .Position = Vector3(-scale.x, -scale.y, 0.0f),
                .Uv       = Vector2(0.0f, 1.0f),
                .Col      = Color::White
            },
            {
                .Position = Vector3(scale.x, -scale.y, 0.0f),
                .Uv       = Vector2(1.0f, 1.0f),
                .Col      = Color::White
            }
        };

        // Update GPU buffer.
        _gpuBuffers.ViewportVertices.UpdateVertices(copyPass, ToSpan(bufferVerts), 0);
        _gpuBuffers.ViewportVertices.UpdateIdxs(copyPass, ToSpan(BUFFER_IDXS), 0);
    }

    void Renderer::PushVertexUniform(const UniformType& uni, int slotIdx)
    {
        std::visit([&, this](auto&& arg)
        {
            SDL_PushGPUVertexUniformData(_commandBuffer, slotIdx, &arg, sizeof(arg));
        }, uni);
    }

    void Renderer::PushFragmentUniform(const UniformType& uni, int slotIdx)
    {
        std::visit([&, this](auto&& arg)
        {
            SDL_PushGPUFragmentUniformData(_commandBuffer, slotIdx, &arg, sizeof(arg));
        }, uni);
    }

    void Renderer::ClearDrawBatches()
    {
        _drawBatches.Primitives2d.clear();
        _drawBatches.Primitives3d.clear();
    }
}

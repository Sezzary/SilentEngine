#include "Framework.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Renderer/Backends/SdlGpu/Pipeline/Pipeline.h"
#include "Renderer/Backends/SdlGpu/Resources/Buffer.h"
#include "Renderer/Backends/SdlGpu/Resources/MeshCache.h"
#include "Renderer/Backends/SdlGpu/Resources/PingPongTexture.h"
#include "Renderer/Backends/SdlGpu/Resources/TextureCache.h"
#include "Renderer/Backends/SdlGpu/Resources/VertexBuffer.h"
#include "Renderer/Common/View.h"
#include "Renderer/Renderer.h"
#include "Services/Options.h"

using namespace Silent::Assets;
using namespace Silent::Services;

namespace Silent::Renderer::SdlGpu
{
    void Renderer::Draw3dScene()
    {
        const auto& options = g_App.GetOptions();

        auto& renderTarget = _renderTargets[(int)RenderTargetType::Native];

        // Start copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        CopyImmediatePrimitives3d(*copyPass);

        // End copy pass.
        SDL_EndGPUCopyPass(copyPass);

        // Start render pass.
        const auto& clearColor      = _scene.Frame.Front.ClearColor;
        auto        colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture     = renderTarget.Write(),
            .clear_color = SDL_FColor{ clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A() },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE
        };
        auto depthTargetInfo = SDL_GPUDepthStencilTargetInfo
        {
            .texture     = _depthTarget,
            .clear_depth = 1.0f,
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_DONT_CARE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, &depthTargetInfo);

        GetMeshes().Bind(renderPass);
        _pipelines.Bind(renderPass, RenderStage::Model, BlendMode::Alpha);

        _view.Move();

        // @temp
        //---------------------------

        const auto* harryAnm = g_App.GetAssets()["Psx/ANIM/HB_BASE.ANM"];
        const auto* harryAnm1 = g_App.GetAssets()["Psx/ANIM/HB_M1S00.ANM"];
        const auto harryAnmData = harryAnm1->GetData<AnmAsset>();

        // @todo How can an optional palette be bound in a clean way?
        auto* tex             = GetTextures()["Psx/CHARA/HERO.TIM"];
        auto* paletteAtlasTex = GetTextures()[Fmt("{}{}", "Psx/CHARA/HERO.TIM", PALETTE_ATLAS_SUFFIX)];
        if (tex != nullptr)
        {
            tex->Bind(renderPass, GetActiveSampler(), 0);
            paletteAtlasTex->Bind(renderPass, GetActiveSampler(), 1);

            auto modelMat = Matrix::Identity;
            modelMat.Rotate(DEG_TO_RAD(180.0f), Vector3::UnitX);
            //modelMat.Translate(harryAnmData->Bones[1].BindTranslation);

            auto viewProj = _view.GetMatrix(glm::radians(0.0f), GetViewportAspectRatio(), 0.1f, 100.0f);

            // Push per-frame 3D primitive uniform.
            auto uniVert0 = UniformPrimitive3dPerFrame
            {
                .ViewProjectionMat   = viewProj,
                .HasJitter           = options->EnableVertexJitter,
                .ViewportAspectRatio = GetViewportAspectRatio(),
            };
            PushVertexUniform(uniVert0, 0);

            // Push per-object 3D primitive uniform.
            auto uniVert1 = UniformPrimitive3dPerObject
            {
                .ModelMat = modelMat
            };
            PushVertexUniform(uniVert1, 1);

            // Push per-object model uniform.
            auto uniFrag0 = UniformModelPerObject
            {
                .IsFastAlpha = false
            };
            PushFragmentUniform(uniFrag0, 0);

            // Draw.
            const auto* mesh = GetMeshes()["Psx/CHARA/HERO.ILM_02HEAD1"];
            if (mesh != nullptr)
            {
                SDL_DrawGPUIndexedPrimitives(&renderPass, mesh->IdxCount, 1, mesh->IdxOffset, mesh->VertexOffset, 0);
                _scene.Frame.Back.DrawCallCount++;
            }
        }

        //---------------------------

        _gpuBuffers.ImmediateVertices3d.Bind(renderPass, 0, 0);
        //auto* tex = GetTextures()["Psx/TIM/BG_ETC.TIM"];
        
        // Draw 3D primitives.
        for (const auto& batch : _drawBatches.Primitives3d)
        {
            // Pipeline setup.
            _pipelines.Bind(renderPass, batch.RenderStg, batch.BlendMd);
            PushFragmentUniform(batch.Uniform, 0);

            auto model = Matrix::Identity;
            model.Rotate(DEG_TO_RAD(45.0f), Vector3::UnitX);

            auto viewProj = _view.GetMatrix(glm::radians(45.0f), GetViewportAspectRatio(), 0.1f, 100.0f);

            // Push per-frame 3D primitive uniform.
            auto uniVert0 = UniformPrimitive3dPerFrame
            {
                .ViewProjectionMat   = viewProj,
                .HasJitter           = options->EnableVertexJitter,
                .ViewportAspectRatio = GetViewportAspectRatio(),
            };
            PushVertexUniform(uniVert0, 0);

            // Push per-object 3D primitive uniform.
            auto uniVert1 = UniformPrimitive3dPerObject
            {
                .ModelMat = model
            };
            PushVertexUniform(uniVert1, 1);

            // Bind texture.
            auto* tex = GetTextures()[batch.TextureName];
            if (tex != nullptr)
            {
                tex->Bind(renderPass, GetActiveSampler(), 0);
            }

            // Draw.
            SDL_DrawGPUIndexedPrimitives(&renderPass, batch.VertexCount, 1, batch.IdxOffset, batch.VertexOffset, 0);
            _scene.Frame.Back.DrawCallCount++;
        }

        // End render pass.
        SDL_EndGPURenderPass(&renderPass);
        renderTarget.Swap();
    }

    void Renderer::Draw3dScenePostProcess()
    {
        const auto& options = g_App.GetOptions();

        // Start copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        CopyGpuViewportQuad(*copyPass);

        // End copy pass.
        SDL_EndGPUCopyPass(copyPass);

        // FXAA.
        if (options->Antialiasing == AntialiasingType::Low)
        {
            RunPostProcessPass(RenderStage::Fxaa, [&]()
            {
                auto uniFrag0 = UniformFxaaPerFrame
                {
                    .Resolution = GetViewportResolution().ToVector2()
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }

        // Dithering.
        if (options->DitheringScale != DitheringScaleType::None)
        {
            RunPostProcessPass(RenderStage::Dither, [&]()
            {
                auto res = GetViewportResolution().ToVector2();

                float virtualHeight = res.y;
                switch(options->DitheringScale)
                {
                    case DitheringScaleType::Retro:
                    {
                        virtualHeight = RETRO_SCREEN_SPACE_RES.y;
                        break;
                    }
                    case DitheringScaleType::Retro2x:
                    {
                        virtualHeight = RETRO_SCREEN_SPACE_RES.y * 2.0f;
                        break;
                    }
                    default:
                    case DitheringScaleType::Native:
                    {
                        break;
                    }
                }

                auto uniFrag0 = UniformDitherPerFrame
                {
                    .Resolution    = res,
                    .VirtualHeight = virtualHeight
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }
    }

    void Renderer::Draw2dScene()
    {
        auto& renderTarget = _renderTargets[(int)RenderTargetType::Native];

        // @todo Additionally draw 3D objects in 3D space.

        // Start copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        // Copy primitives.
        CopyImmediatePrimitives2d(*copyPass);

        // Copy 3D scene to write texture.
        auto viewportRes = GetViewportResolution();
        auto fromTexLoc  = SDL_GPUTextureLocation{ .texture = renderTarget.Read()  };
        auto toTexLoc    = SDL_GPUTextureLocation{ .texture = renderTarget.Write() };
        SDL_CopyGPUTextureToTexture(copyPass, &fromTexLoc, &toTexLoc, viewportRes.x, viewportRes.y, 1, false);

        // End copy pass.
        SDL_EndGPUCopyPass(copyPass);

        // Start render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = renderTarget.Write(),
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto depthTargetInfo = SDL_GPUDepthStencilTargetInfo
        {
            .texture     = _depthTarget,
            .clear_depth = 1.0f,
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_DONT_CARE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, &depthTargetInfo);

        // Bind default texture and palette atlas.
        auto* tex = GetTextures()[""];
        tex->Bind(renderPass, GetActiveSampler(), 0);
        tex->Bind(renderPass, GetActiveSampler(), 1);

        // Draw 2D primitives.
        _gpuBuffers.ImmediateVertices2d.Bind(renderPass, 0, 0);
        for (const auto& batch : _drawBatches.Primitives2d)
        {
            // Bind pipeline.
            _pipelines.Bind(renderPass, batch.RenderStg, batch.BlendMd);

            // Bind texture and optional palette atlas.
            if (!batch.TextureName.empty())
            {
                tex = GetTextures()[batch.TextureName];
                if (tex != nullptr)
                {
                    tex->Bind(renderPass, GetActiveSampler(), 0);
                }
                tex = GetTextures()[batch.TextureName + PALETTE_ATLAS_SUFFIX];
                if (tex != nullptr)
                {
                    tex->Bind(renderPass, GetActiveSampler(), 1);
                }
            }

            // Push uniform.
            PushFragmentUniform(batch.Uniform, 0);

            // Draw.
            SDL_DrawGPUIndexedPrimitives(&renderPass, batch.VertexCount, 1, batch.IdxOffset, batch.VertexOffset, 0);
            _scene.Frame.Back.DrawCallCount++;
        }

        // End render pass.
        SDL_EndGPURenderPass(&renderPass);
        renderTarget.Swap();
    }

    void Renderer::DrawScenePostProcess()
    {
        const auto& options = g_App.GetOptions();

        // @debug Test timer.
        static float time = 0.0f;
        time              = fmod(time + 0.1f, 1.0f);

        // Start copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        CopyGpuViewportQuad(*copyPass);

        // End copy pass.
        SDL_EndGPUCopyPass(copyPass);

        // Luma fade.
        if (_scene.Frame.Front.LumaFadeAlpha > 0.0f)
        {
            RunPostProcessPass(RenderStage::LumaFade, [&]()
            {
                auto uniFrag0 = UniformLumaFadePerFrame
                {
                    .FadeAlpha = _scene.Frame.Front.LumaFadeAlpha,
                    .IsWhite   = _scene.Frame.Front.IsLumaFadeWhite
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }

        // Film grain.
        if (options->EnableFilmGrain)
        {
            RunPostProcessPass(RenderStage::FilmGrain, [&]()
            {
                auto uniFrag0 = UniformFilmGrainPerFrame
                {
                    .Time = time
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }

        // @todo Need largest render target.

        // Vignette.
        if (options->EnableVignette)
        {
            RunPostProcessPass(RenderStage::Vignette, [&]()
            {
                auto uniFrag0 = UniformVignettePerFrame
                {
                    .Resolution = GetViewportResolution().ToVector2(),
                    .Time       = time
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }

        // CRT filter.
        if (options->EnableCrtFilter)
        {
            RunPostProcessPass(RenderStage::Crt, [&]()
            {
                auto uniFrag0 = UniformCrtPerFrame
                {
                    .Resolution = GetViewportResolution().ToVector2(),
                    .Time       = time
                };
                PushFragmentUniform(uniFrag0, 0);
            });
        }
    }

    void Renderer::DrawViewport()
    {
        constexpr float BRIGHTNESS_STEP   = 0.25f / BRIGHTNESS_LEVEL_MAX;
        constexpr float BRIGHTNESS_MIDDLE = BRIGHTNESS_STEP * (BRIGHTNESS_LEVEL_MAX / 2);

        const auto& options = g_App.GetOptions();

        auto& renderTarget = _renderTargets[(int)RenderTargetType::Native];

        // Start render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture     = _swapchainTarget,
            .clear_color = SDL_FColor{ Color::Black.R(), Color::Black.G(), Color::Black.B(), Color::Black.A() },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Bind pipeline.
        _pipelines.Bind(renderPass, RenderStage::Blit, BlendMode::Opaque, false);

        // Bind vertex buffer.
        _gpuBuffers.ViewportVertices.Bind(renderPass, 0, 0);

        // Bind texture.
        auto binding = SDL_GPUTextureSamplerBinding
        {
            .texture = renderTarget.Read(),
            .sampler = _samplers[(int)TextureFilterType::Nearest]
        };
        SDL_BindGPUFragmentSamplers(&renderPass, 0, &binding, 1);

        // Push per-frame BLIT uniform. @todo Brightness is only applied to the 3D scene, not the entire viewport.
        auto uniFrag0 = UniformBlitPerFrame
        {
            .Brightness = (options->BrightnessLevel * BRIGHTNESS_STEP) - BRIGHTNESS_MIDDLE
        };
        PushFragmentUniform(uniFrag0, 0);

        // Draw.
        SDL_DrawGPUIndexedPrimitives(&renderPass, QUAD_IDX_COUNT, 1, 0, 0, 0);
        _scene.Frame.Back.DrawCallCount++;

        // End render pass.
        SDL_EndGPURenderPass(&renderPass);
    }

    void Renderer::DrawDebugMenu()
    {
        // Start new frame.
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Draw GUIs.
        for (auto& drawCall : _scene.Frame.Front.DebugGuiDrawCalls)
        {
            drawCall();
        }

        // Prepare render data.
        ImGui::Render();
        auto* drawData = ImGui::GetDrawData();
        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, _commandBuffer);

        // Start render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTarget,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Draw.
        ImGui_ImplSDLGPU3_RenderDrawData(drawData, _commandBuffer, renderPass);
        _scene.Frame.Back.DrawCallCount++;

        // End render pass.
        SDL_EndGPURenderPass(renderPass);
    }

    void Renderer::RunPostProcessPass(RenderStage renderStage, const std::function<void()>& pushUniforms)
    {
        auto& renderTarget = _renderTargets[(int)RenderTargetType::Native];

        // Process render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = renderTarget.Write(),
            .load_op  = SDL_GPU_LOADOP_DONT_CARE,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Bind pipeline.
        _pipelines.Bind(*renderPass, renderStage, BlendMode::Opaque, false);

        // Bind vertex buffer.
        _gpuBuffers.ViewportVertices.Bind(*renderPass, 0, 0);

        // Bind texture.
        auto binding = SDL_GPUTextureSamplerBinding
        {
            .texture = renderTarget.Read(),
            .sampler = _samplers[(int)TextureFilterType::Nearest]
        };
        SDL_BindGPUFragmentSamplers(renderPass, 0, &binding, 1);

        // Push uniforms.
        pushUniforms();

        // Draw.
        SDL_DrawGPUIndexedPrimitives(renderPass, QUAD_IDX_COUNT, 1, 0, 0, 0);
        _scene.Frame.Back.DrawCallCount++;

        // End render pass.            
        SDL_EndGPURenderPass(renderPass);
        renderTarget.Swap();
    }
}

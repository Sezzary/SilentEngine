#include "Framework.h"
#include "Renderer/Renderer.h"

#include "Application.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"
#include "Renderer/Common/Resources/Primitive/Primitive3d.h"
#include "Renderer/Common/Resources/Primitive/Vertex2d.h"
#include "Renderer/Common/Resources/Primitive/Vertex3d.h"
#include "Renderer/Common/Resources/Scene/Shape2d.h"
#include "Renderer/Common/Resources/Scene/Sprite2d.h"
#include "Renderer/Common/Resources/Scene/Text2d.h"
#include "Renderer/Common/Resources/Scene/Triangle3d.h"
#include "Renderer/Common/Utils.h"
#include "Renderer/Common/View.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Renderer
{
    RendererType RendererBase::GetType() const
    {
        return _type;
    }

    int RendererBase::GetDrawCallCount() const
    {
        return _scene.Frame.Front.DrawCallCount;
    }

    void RendererBase::SetClearColor(const Color& color)
    {
        _scene.Frame.Back.ClearColor = color;
    }

    void RendererBase::SetLumaFade(float alpha, bool isWhite)
    {
        _scene.Frame.Back.LumaFadeAlpha   = std::clamp(alpha, 0.0f, 1.0f);
        _scene.Frame.Back.IsLumaFadeWhite = isWhite;
    }

    Vector2i RendererBase::GetViewportResolution() const
    {
        const auto& options = g_App.GetOptions();

        auto res = _scene.Frame.Front.SwapchainResolution.ToVector2();

        float virtualHeight = res.y;
        switch (options->RenderScale)
        {
            case RenderScaleType::Retro:
            {
                virtualHeight = RETRO_SCREEN_SPACE_RES.y;
                break;
            }
            case RenderScaleType::Retro2x:
            {
                virtualHeight = RETRO_SCREEN_SPACE_RES.y * 2.0f;
                break;
            }
            default:
            case RenderScaleType::Native:
            {
                break;
            }
        }

        float aspect = GetViewportAspectRatio();
        return Vector2i(virtualHeight * aspect, virtualHeight);
    }

    float RendererBase::GetViewportAspectRatio() const
    {
        const auto& options = g_App.GetOptions();

        auto res = _scene.Frame.Front.SwapchainResolution.ToVector2();

        // Compute aspect ratio.
        float aspect = res.x / res.y;
        switch (options->AspectRatio)
        {
            case AspectRatioType::Retro:
            {
                aspect = ASPECT_RATIO_4_TO_3;
                break;
            }
            case AspectRatioType::Wide:
            {
                aspect = ASPECT_RATIO_16_TO_9;
                break;
            }
            default:
            case AspectRatioType::Native:
            {
                break;
            }
        }

        return aspect;
    }

    const BoundingFrustum& RendererBase::GetViewFrustum() const
    {
        return _view.Frustum;
    }

    void RendererBase::UpdateView()
    {
        // @todo
    }

    void RendererBase::PrepareFrameData()
    {
        auto& video    = g_App.GetVideo();
        auto& executor = g_App.GetExecutor();

        _scene.Frame.Back.SwapchainResolution = g_App.GetWindowResolution();

        // @todo Using parallelism here causes flickering, but even without it some 2D objects don't draw. There's
        // a severe bug somewhere.
        // Generate active buffer data.
        //auto tasks = ParallelTasks
        //{
        //    TASK(ProcessShapes2d()),
        //    TASK(ProcessSprites2d()),
        //    TASK(ProcessGlyphs2d()),
        //    TASK(ProcessTriangles3d())
        //};
        //executor.AddTasks(tasks).wait();
        ProcessShapes2d();
        ProcessSprites2d();
        ProcessGlyphs2d();
        ProcessTriangles3d();

        _scene.Frame.Swap();
        _scene.Frame.Back.IsResized     = false;
        _scene.Frame.Back.DrawCallCount = 0;
        _scene.Frame.Back.ImmediatePrimitives2d.clear();
        _scene.Frame.Back.ImmediatePrimitives3d.clear();
        _scene.Frame.Back.DebugGuiDrawCalls.clear();
        _scene.Frame.Back.TextureUploadQueue.clear();
        _scene.Frame.Back.TextureReleaseQueue.clear();
        _scene.Frame.Back.MeshUploadQueue.clear();
        _scene.Frame.Back.MeshReleaseQueue.clear();

        video.SwapFrameBuffer();
    }

    void RendererBase::SignalResize()
    {
        _scene.Frame.Back.IsResized = true;
    }

    void RendererBase::QueueTextureUpload(const std::string& assetName)
    {
        _scene.Frame.Back.TextureUploadQueue.push_back(assetName);
    }

    void RendererBase::QueueTextureRelease(const std::string& assetName)
    {
        _scene.Frame.Back.TextureReleaseQueue.push_back(assetName);
    }

    void RendererBase::QueueMeshUpload(const std::string& assetName)
    {
        _scene.Frame.Back.MeshUploadQueue.push_back(assetName);
    }

    void RendererBase::QueueMeshRelease(const std::string& assetName)
    {
        _scene.Frame.Back.MeshReleaseQueue.push_back(assetName);
    }

    bool RendererBase::SubmitShape2d(const Shape2d& shape)
    {
        if (_scene.Objects.Shapes2d.size() >= SHAPE_2D_COUNT_MAX)
        {
            Debug::Log("Attempted to submit 2D shape to full container.",
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return false;
        }

        _scene.Objects.Shapes2d.push_back(shape);
        return true;
    }

    bool RendererBase::SubmitSprite2d(const Sprite2d& sprite)
    {
        auto& assets = g_App.GetAssets();

        if (_scene.Objects.Sprites2d.size() >= SPRITE_2D_COUNT_MAX)
        {
            Debug::Log("Attempted to submit 2D sprite to full container.",
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return false;
        }

        // @todo Handle this somehow somewhere, but not here. Sprite textures don't
        // have to come from streamable assets, e.g. font atlases.
        //const auto* asset = assets[sprite.TextureName];
        //if (asset->Type != AssetType::Tim)
        //{
        //    Debug::Log("Attempted to submit non-image asset as screen sprite.",
        //               Debug::LogLevel::Warning, Debug::LogMode::Debug);
        //    return false;
        //}

        _scene.Objects.Sprites2d.push_back(sprite);
        return true;
    }

    bool RendererBase::SubmitText2d(const Text2d& text)
    {
        constexpr auto    SHADOW_COLOR  = Color::From8Bit(16, 16, 16);
        static const auto SHADOW_OFFSET = SCREEN_SPACE_RES / Vector2(RETRO_SCREEN_SPACE_RES.y);

        // Compute transformation parameters.
        auto  rotMat           = Matrix::CreateRotationZ(text.Rotation);
        auto  fontScaleFactor  = SCREEN_SPACE_RES / (float)text.Font->GetPointSize();
        float heightScale      = (text.StyleFlags & (int)TextStyleFlags::HalfHeight) ? 0.5f : 1.0f;
        auto  textScale        = Vector2(1.0f, heightScale) * text.Scale;
        auto  textSize         = (Vector2(text.Shape.Width, (float)text.Font->GetPointSize()) * fontScaleFactor) * textScale;
        auto  aspectCorrection = GetScreenAspectCorrection(GLYPH_SCALE_MODE);

        // Compute text position.
        auto textOffset = Vector2::Zero;
        switch (text.AlignMd)
        {
            case AlignMode::Center:
            {
                textOffset = Vector2(-textSize.x, textSize.y) * 0.5f;
                break;
            }
            case AlignMode::CenterTop:
            {
                textOffset = Vector2(-textSize.x * 0.5f, textSize.y);
                break;
            }
            case AlignMode::CenterBottom:
            {
                textOffset = Vector2(-textSize.x * 0.5f, 0.0f);
                break;
            }
            case AlignMode::CenterLeft:
            {
                textOffset = Vector2(0.0f, textSize.y * 0.5f);
                break;
            }
            case AlignMode::CenterRight:
            {
                textOffset = Vector2(-textSize.x, textSize.y * 0.5f);
                break;
            }
            case AlignMode::TopLeft:
            {
                textOffset = Vector2(0.0f, textSize.y);
                break;
            }
            case AlignMode::TopRight:
            {
                textOffset = Vector2(-textSize.x, textSize.y);
                break;
            }
            default:
            case AlignMode::BottomLeft:
            {
                break;
            }
            case AlignMode::BottomRight:
            {
                textOffset = Vector2(-textSize.x, 0.0f);
                break;
            }
        }
        auto adjTextPos = text.Position + (Vector2::Transform(textOffset, rotMat) * aspectCorrection);

        // Compute shadow offset.
        auto shadowOffset    = (SHADOW_OFFSET * Vector2(1.0f, heightScale)) * aspectCorrection;
        auto adjShadowOffset = Vector2::Transform(shadowOffset, rotMat);
        // @todo This version scales according to the internal pixel size of the font.
        //auto shadowOffset    = ((SHADOW_RETRO_PIXEL_OFFSET * fontScaleFactor) * textScale) * aspectCorrection;
        //auto adjShadowOffset = Vector2::Transform(shadowOffset, rotMat);

        // Run through shaped glyphs.
        auto pixelOffset = Vector2::Zero;
        for (const auto& shapedGlyph : text.Shape.Glyphs)
        {
            // Compute texture atlas UVs.
            auto uvMin = shapedGlyph.Attribs.AtlasPosition.ToVector2() / Vector2(Font::ATLAS_SIZE); 
            auto uvMax = uvMin + (shapedGlyph.Attribs.AtlasSize.ToVector2() / Vector2(Font::ATLAS_SIZE));

            // Compute rotated offset.
            auto adjPixelOffset = Vector2::Transform(pixelOffset, rotMat);

            // Compute rotated bearing.
            auto pixelBearing    = Vector2(shapedGlyph.Attribs.Bearing.x,
                                           shapedGlyph.Attribs.AtlasSize.y - shapedGlyph.Attribs.Bearing.y);
            auto adjPixelBearing = Vector2::Transform(pixelBearing, rotMat);

            // Compute screen position.
            auto relPixelPos = adjPixelOffset + adjPixelBearing;
            auto relPos      = (relPixelPos * fontScaleFactor) * textScale;
            auto pos         = adjTextPos + (relPos * aspectCorrection);

            // Compute scale.
            auto relScale = Vector2((float)shapedGlyph.Attribs.AtlasSize.x / (float)shapedGlyph.Attribs.AtlasSize.y, 1.0f) *
                            Vector2((float)shapedGlyph.Attribs.AtlasSize.y / (float)text.Font->GetPointSize());
            auto scale    = relScale * textScale;

            // Concatenate name for texture atlas containing glyph.
            auto atlasName = text.Font->GetName() + std::to_string(shapedGlyph.Attribs.AtlasIdx);

            auto AddGlyph = [&](const Vector2& offset, const Color& color, int depth, bool hasGradient)
            {
                if (_scene.Objects.Glyphs2d.size() >= GLYPH_2D_COUNT_MAX)
                {
                    Debug::Log("Attempted to submit 2D glyph to full container.",
                               Debug::LogLevel::Warning, Debug::LogMode::Debug);
                    return false;
                }

                auto glyph = Glyph2d::CreateGlyph2d(shapedGlyph, hasGradient,
                                                    atlasName, uvMin, uvMax,
                                                    pos + offset, text.Rotation, scale, color,
                                                    depth);
                _scene.Objects.Glyphs2d.push_back(glyph);

                return true;
            };

            // Submit 2D glyph.
            if (!AddGlyph(Vector2::Zero, text.Col, text.Depth, text.StyleFlags & (int)TextStyleFlags::Gradient))
            {
                return false;
            }

            // Submit 2D drop shadow glyph.
            if (text.StyleFlags & (int)TextStyleFlags::Shadow)
            {
                auto shadowColor = SHADOW_COLOR;
                shadowColor.A()  = text.Col.A();
                if (!AddGlyph(adjShadowOffset, shadowColor, text.Depth + 1, false))
                {
                    return false;
                }
            }

            // Update horizontal offset.
            pixelOffset.x += shapedGlyph.Spacing;
        }

        return true;
    }

    std::unique_ptr<RendererBase> CreateRenderer(RendererType type)
    {
        switch (type)
        {
            case RendererType::SdlGpu:
            {
                return std::make_unique<SdlGpu::Renderer>();
            }
        }

        return nullptr;
    }
}

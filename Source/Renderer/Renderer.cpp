#include "Framework.h"
#include "Renderer/Renderer.h"

#include "Application.h"
#include "Renderer/Common/Objects/Primitive/Vertex2d.h"
#include "Renderer/Common/Objects/Primitive/Vertex3d.h"
#include "Renderer/Common/Objects/Primitive2d.h"
#include "Renderer/Common/Objects/Primitive3d.h"
#include "Renderer/Common/Objects/Scene/Sprite2d.h"
#include "Renderer/Common/Objects/Scene/Text.h"
#include "Renderer/Backends/OpenGl/OpenGl.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"

namespace Silent::Renderer
{
    RendererType RendererBase::GetType() const
    {
        return _type;
    }

    uint RendererBase::GetDrawCallCount() const
    {
        return _drawCallCount;
    }

    void RendererBase::SetClearColor(const Color& color)
    {
        _clearColor = color;
    }

    Vector2i RendererBase::GetScreenResolution() const
    {
        constexpr auto RETRO_RES = Vector2i(320, 240);

        const auto& options = g_App.GetOptions();

        auto res = g_App.GetWindowResolution();
        return res;

        // @todo Render scale should be a post-process instead?
        switch (options->RenderScale)
        {
            case RenderScaleType::Native:
            {
                break;
            }
            case RenderScaleType::Half:
            {
                res /= 2;
                break;
            }
            case RenderScaleType::Quarter:
            {
                res /= 4;
                break;
            }
            case RenderScaleType::Retro:
            {
                res = RETRO_RES;
                break;
            }
        }

        return res;
    }

    void RendererBase::SignalResize()
    {
        _isResized = true;
    }

    void RendererBase::Submit2dPrimitive(const Primitive2d& prim)
    {
        if (_primitives2d.size() >= PRIMITIVE_2D_COUNT_MAX)
        {
            Log("Attampted to add 2D primitive to full container.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        _primitives2d.push_back(prim);
    }

    void RendererBase::SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                          const Color& color, int depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode)
    {
        auto& assets = g_App.GetAssets();

        const auto asset = assets.GetAsset(assetIdx);
        if (asset->Type != AssetType::Tim)
        {
            Log("Attempted to submit non-image asset as screen sprite.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // @todo
    }

    void RendererBase::SubmitDebugText(const std::string& msg, const Vector2& pos, const Color& color, TextAlignMode alignMode)
    {
        // @todo
    }

    void RendererBase::SubmitDebugGui(std::function<void()> drawFunc)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode)
        {
            return;
        }

        _debugGuiDrawCalls.push_back(drawFunc);
    }

    void RendererBase::SubmitDebugLine(const Vector3& from, const Vector3& to, const Color& color, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        auto line = Primitive3d::CreateDebugLine(from, to, color);
        _debugPrimitives3d.push_back(line);
    }

    void RendererBase::SubmitDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        auto tri = Primitive3d::CreateDebugTriangle(vert0, vert1, vert2, color);
        _debugPrimitives3d.push_back(tri);
    }

    void RendererBase::SubmitDebugTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        auto rotMatrix = rot.ToRotationMatrix();

        auto from0 = center + Vector3::Transform(Vector3(radius,  0.0f, 0.0f), rotMatrix);
        auto to0   = center + Vector3::Transform(Vector3(-radius, 0.0f, 0.0f), rotMatrix);
        SubmitDebugLine(from0, to0, color, page);

        auto from1 = center + Vector3::Transform(Vector3(0.0f,  radius, 0.0f), rotMatrix);
        auto to1   = center + Vector3::Transform(Vector3(0.0f, -radius, 0.0f), rotMatrix);
        SubmitDebugLine(from1, to1, color, page);

        auto from2 = center + Vector3::Transform(Vector3(0.0f, 0.0f,  radius), rotMatrix);
        auto to2   = center + Vector3::Transform(Vector3(0.0f, 0.0f, -radius), rotMatrix);
        SubmitDebugLine(from2, to2, color, page);
    }

    void RendererBase::SubmitDebugBox(const OrientedBoundingBox& box, const Color& color, bool isWireframe, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        auto corners = box.GetCorners();

        // Wireframe.
        if (isWireframe)
        {
            SubmitDebugLine(corners[0], corners[1], color, page);
            SubmitDebugLine(corners[1], corners[2], color, page);
            SubmitDebugLine(corners[2], corners[3], color, page);
            SubmitDebugLine(corners[3], corners[0], color, page);

            SubmitDebugLine(corners[4], corners[5], color, page);
            SubmitDebugLine(corners[5], corners[6], color, page);
            SubmitDebugLine(corners[6], corners[7], color, page);
            SubmitDebugLine(corners[7], corners[4], color, page);

            SubmitDebugLine(corners[0], corners[4], color, page);
            SubmitDebugLine(corners[1], corners[5], color, page);
            SubmitDebugLine(corners[2], corners[6], color, page);
            SubmitDebugLine(corners[3], corners[7], color, page);
        }
        // Solid.
        else
        {
            SubmitDebugTriangle(corners[0], corners[1], corners[2], color, page);
            SubmitDebugTriangle(corners[0], corners[2], corners[3], color, page);

            SubmitDebugTriangle(corners[4], corners[5], corners[6], color, page);
            SubmitDebugTriangle(corners[4], corners[6], corners[7], color, page);

            SubmitDebugTriangle(corners[0], corners[1], corners[4], color, page);
            SubmitDebugTriangle(corners[1], corners[4], corners[5], color, page);

            SubmitDebugTriangle(corners[1], corners[2], corners[5], color, page);
            SubmitDebugTriangle(corners[2], corners[5], corners[6], color, page);

            SubmitDebugTriangle(corners[2], corners[3], corners[6], color, page);
            SubmitDebugTriangle(corners[3], corners[6], corners[7], color, page);

            SubmitDebugTriangle(corners[0], corners[3], corners[4], color, page);
            SubmitDebugTriangle(corners[3], corners[4], corners[7], color, page);
        }
    }

    void RendererBase::SubmitDebugSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, DebugPage page)
    {
        constexpr uint WIREFRAME_SEGMENT_COUNT = 24;

        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        // Wireframe.
        if (isWireframe)
        {
            // Draw circles in XY, YZ, and XZ planes.
            for (int plane = 0; plane < Vector3::AXIS_COUNT; plane++)
            {
                for (int i = 0; i < WIREFRAME_SEGMENT_COUNT; i++)
                {
                    float theta0 = (((float)i / WIREFRAME_SEGMENT_COUNT) * 2.0f) * PI;
                    float theta1 = (((float)(i + 1) / WIREFRAME_SEGMENT_COUNT) * 2.0f) * PI;

                    auto point0 = Vector3::Zero;
                    auto point1 = Vector3::Zero;
                    switch (plane)
                    {
                        // XY plane.
                        case 0:
                        {
                            point0 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta0), sphere.Radius * glm::sin(theta0), 0.0f);
                            point1 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta1), sphere.Radius * glm::sin(theta1), 0.0f);
                            break;
                        }
                        // YZ plane.
                        case 1:
                        {
                            point0 = sphere.Center + Vector3(0.0f, sphere.Radius * glm::cos(theta0), sphere.Radius * glm::sin(theta0));
                            point1 = sphere.Center + Vector3(0.0f, sphere.Radius * glm::cos(theta1), sphere.Radius * glm::sin(theta1));
                            break;
                        }
                        // ZX plane.
                        case 2:
                        {
                            point0 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta0), 0.0f, sphere.Radius * glm::sin(theta0));
                            point1 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta1), 0.0f, sphere.Radius * glm::sin(theta1));
                            break;
                        }
                    }
                    SubmitDebugLine(point0, point1, color, page);
                }
            }
        }
        // Solid.
        else
        {
            // TOO
        }
    }

    void RendererBase::SubmitDebugCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void RendererBase::SubmitDebugCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void RendererBase::SubmitDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode || (page != g_DebugData.Page && page != DebugPage::None))
        {
            return;
        }

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void RendererBase::PrepareFrameData()
    {
        // @todo Intermediate data -> renderer-ready data. At later stages, outside this method, renderer-ready data -> GPU copy-ready data.
    }

    void RendererBase::ClearFrameData()
    {
        _drawCallCount = 0;

        _primitives2d.clear();
        _sprites2d.clear();
        _debugPrimitives3d.clear();
        _debugGuiDrawCalls.clear();
    }

    std::unique_ptr<RendererBase> CreateRenderer(RendererType type)
    {
        switch (type)
        {
            case RendererType::OpenGl:
            {
                return std::make_unique<OpenGlRenderer>();
            }
            case RendererType::SdlGpu:
            {
                return std::make_unique<SdlGpuRenderer>();
            }
        }

        return nullptr;
    }
}

#include "Framework.h"
#include "Renderer/Renderer.h"

#include "Application.h"
#include "Renderer/Common/Resources/Scene/Shape2d.h"
#include "Renderer/Common/Resources/Scene/Triangle3d.h"

namespace Silent::Renderer
{
    void RendererBase::SubmitDebugGui(std::function<void()> drawFunc)
    {
        if (!g_App.IsDrawTick())
        {
            return;
        }

        if (_scene.Frame.Back.DebugGuiDrawCalls.size() >= DEBUG_GUI_COUNT_MAX)
        {
            Debug::Log("Attempted to submit debug GUI draw call to full container.",
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }

        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugMode)
        {
            return;
        }

        _scene.Frame.Back.DebugGuiDrawCalls.push_back(drawFunc);
    }

    void RendererBase::SubmitDebugLine(const Vector2& from, const Vector2& to, const Color& color, ScaleMode scaleMode,
                                       Debug::Page page)
    {
        if (!Debug::CheckPage(page) || !g_App.IsDrawTick())
        {
            return;
        }

        auto line = Shape2d::CreateLine(from, to, color, color, 0, scaleMode, BlendMode::Add);
        _scene.Objects.Shapes2d.push_back(line);
    }

    void RendererBase::SubmitDebugLine(const Vector3& from, const Vector3& to, const Color& color, Debug::Page page)
    {
        if (!Debug::CheckPage(page) || !g_App.IsDrawTick())
        {
            return;
        }

        // @todo Submit to `_scene.Objects.Triangles3d`.
        auto line = Primitive3d::CreateDebugLine(from, to, color);
        //_scene.Frame.Back.Primitives3d.push_back(line);
    }

    void RendererBase::SubmitDebugTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                           const Color& color, ScaleMode scaleMode, Debug::Page page)
    {
        if (!Debug::CheckPage(page) || !g_App.IsDrawTick())
        {
            return;
        }

        auto tri = Shape2d::CreateTriangle(vert0, vert1, vert2, color, color, color, 0, scaleMode, BlendMode::Add);
        _scene.Objects.Shapes2d.push_back(tri);
    }

    void RendererBase::SubmitDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2,
                                           const Color& color, Debug::Page page)
    {
        if (!Debug::CheckPage(page) || !g_App.IsDrawTick())
        {
            return;
        }

        auto tri = Triangle3d::CreateTriangle3d(vert0, vert1, vert2, color, NO_VALUE, BlendMode::Add);
        _scene.Objects.Triangles3d.push_back(tri);
    }
}

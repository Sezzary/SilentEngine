#include "Framework.h"
#include "Gui/Cursor.h"

#include "Application.h"
#include "Input/Input.h"
#include "Renderer/Common/Resources/Scene/Sprite2d.h"
#include "Renderer/Common/Utils.h"
#include "Renderer/Renderer.h"

using namespace Silent::Input;
using namespace Silent::Renderer;

namespace Silent::Gui
{
    const Vector2& Cursor::GetPosition() const
    {
        return _position;
    }

    bool Cursor::IsEnabled() const
    {
        return _isEnabled;
    }

    void Cursor::Update()
    {
        const auto& input = g_App.GetInput();

        _isEnabled = input.IsUsingMouse();
        _position  = GetGridAlignedScreenPercent(input.GetCursorPosition(), g_App.GetWindowResolution().y);
    }

    void Cursor::Draw()
    {
        constexpr auto  TEX_RES                   = Vector2i(128, 256);
        constexpr auto  CURSOR_GRAPHIC_RES        = Vector2i(32, 32);
        constexpr auto  CURSOR_GRAPHIC_REGION_POS = Vector2i(0, 64);
        constexpr auto  UV_MIN                    = Vector2((float)CURSOR_GRAPHIC_REGION_POS.x / (float)TEX_RES.x,
                                                            (float)CURSOR_GRAPHIC_REGION_POS.y / (float)TEX_RES.y);
        constexpr auto  UV_MAX                    = Vector2((float)(CURSOR_GRAPHIC_REGION_POS.x + CURSOR_GRAPHIC_RES.x) /
                                                            (float)TEX_RES.x,
                                                            (float)(CURSOR_GRAPHIC_REGION_POS.y + CURSOR_GRAPHIC_RES.y) /
                                                            (float)TEX_RES.y);
        constexpr float SCALE                     = (float)(CURSOR_GRAPHIC_RES.y * RETRO_PIXEL_SCALE.y);
        constexpr float ASPECT                    = (float)CURSOR_GRAPHIC_RES.x / (float)CURSOR_GRAPHIC_RES.y;
        constexpr int   DEPTH                     = 0;

        auto& renderer = g_App.GetRenderer();

        // Submit cursor sprite. @todo Uses font atlas texture???
        auto sprite = Sprite2d::CreateSprite2d("Psx/TIM/BG_ETC.TIM", UV_MIN, UV_MAX,
                                               GetPosition(), DEG_TO_RAD(0.0f), SCALE, ASPECT,
                                               Color::White, 0,
                                               DEPTH, AlignMode::TopLeft, ScaleMode::VerticalEdge,
                                               BlendMode::Alpha);
        renderer.SubmitSprite2d(sprite);
    }
}

#include "Framework.h"
#include "Debug/Scratchpad.h"

#include "Application.h"
#include "Debug/Debug.h"
#include "Input/Input.h"
#include "Renderer/Common/Enums.h"
#include "Renderer/Common/Resources/Scene/Text2d.h"
#include "Renderer/Renderer.h"
#include "Services/Options.h"
#include "Utils/Parallel.h"

#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Bodyprog/Events/MapMsg.h"
#include "Game/Bodyprog/Text/TextDraw.h"

using namespace Silent::Game;

using namespace Silent::Input;
using namespace Silent::Services;

// `Scratchpad` includes.
#include "Gui/Button.h"

using namespace Silent::Gui;

namespace Silent::Debug
{
    Vector2 SnapToGrid(const Vector2& pos, const Vector2& res, float virtualHeight)
    {
        float rawScale = res.y / virtualHeight;
        float scale    = std::max(1.0f, floorf(rawScale));

        //auto gridSize = Vector2(res.x / scale, virtualHeight); // Precise horizontal scaling.
        auto gridSize = Vector2(res.x / rawScale, virtualHeight);

        auto uv = pos / 100.0f;

        auto snappedPixel = Vector2(floorf(uv.x * gridSize.x), 
                                    floorf(uv.y * gridSize.y));

        return Vector2((snappedPixel.x / gridSize.x) * 100.0f,
                       (snappedPixel.y / gridSize.y) * 100.0f);
    }

    void Scratchpad()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            const auto& options    = g_App.GetOptions();
            const auto& translator = g_App.GetTranslator();
            auto&       input      = g_App.GetInput();
            auto&       renderer   = g_App.GetRenderer();
            auto&       fonts      = g_App.GetFonts();

            auto fontName = (options->TextQuality == TextQualityType::Retro) ? "RetroSerif" : "ModernSerif";
            //auto msg = GetParsedMsg("{L0}""打撃系の武器は、攻撃ボタンの押し方に{N}よって攻撃方法が変化します。",
            //                        fontName, 100.0f / 14.0f);
            auto msg = GetParsedMsg("{L0}""{J0(3.0)}{T}{C0}The {C1}phones {C2}are {C3}all {C4}dead,{N}{T}{C7}and the radio, too.",
                                    fontName, 100.0f / 14.0f);
            //auto msg = GetParsedMsg("{L0}" + translator("m0S01Msg_26"),
            //                        fontName, 100.0f / 14.0f);

            int styleFlags = (int)TextStyleFlags::Gradient |
                         (int)TextStyleFlags::Shadow;
            DrawParsedMsg(msg, Vector2::Zero, SERIF_FONT_SCALE, styleFlags);

            return;

            /*float s = 5.0f;
            auto col = Color(0.4f,0.4f,0);
            auto pg = Debug::Page::Renderer;

            // Front Face (Z = 5.0)
            renderer.SubmitDebugTriangle({-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, col, pg); // Tri 1

            // Back Face (Z = -5.0)
            renderer.SubmitDebugTriangle({ s, -s, -s}, {-s, -s, -s}, {-s,  s, -s}, col, pg); // Tri 1

            // Right Face (X = 5.0)
            renderer.SubmitDebugTriangle({ s, -s,  s}, { s, -s, -s}, { s,  s, -s}, col, pg); // Tri 1

            // Left Face (X = -5.0)
            renderer.SubmitDebugTriangle({-s, -s, -s}, {-s, -s,  s}, {-s,  s,  s}, col, pg); // Tri 1

            // Top Face (Y = 5.0)
            renderer.SubmitDebugTriangle({-s,  s,  s}, { s,  s,  s}, { s,  s, -s}, col, pg); // Tri 1

            // Bottom Face (Y = -5.0)
            renderer.SubmitDebugTriangle({-s, -s, -s}, { s, -s, -s}, { s, -s,  s}, col, pg); // Tri 1*/

            //return;

            Msg("Does this work?");

            // ====================================

            Options_MainOptionsMenu_EntryStringsDraw();

            // Sprite test.

            // Cursor.
            auto cursorPos    = SnapToGrid(input.GetCursorPosition(), g_App.GetWindowResolution().ToVector2(), RETRO_SCREEN_SPACE_RES.y);
            auto cursorSprite = Sprite2d::CreateSprite2d("TIM/BG_ETC.TIM", Vector2(0.0f, 64.0f / 256.0f), Vector2(32.0f / 128.0f, 96.0f / 256.0f),
                                                         SCREEN_SPACE_RES / 2.0f, 0.0f, 1.0f, 0.75f, Color::White, NO_VALUE,
                                                         0, AlignMode::Center, ScaleMode::HorizontalEdge, BlendMode::Alpha);
            renderer.SubmitSprite2d(cursorSprite);

            //return;

            //auto sprite0 = Sprite2d::CreateSprite2d("TIM/HERO_PIC.TIM", Vector2::Zero, Vector2::One,
            //                                        Vector2(50.0f, 50.0f), 0.0f, Vector2::One, Color::White, 0,
            //                                        1100, AlignMode::Center, ScaleMode::VerticalEdge, BlendMode::Opaque);
            //renderer.SubmitSprite2d(sprite0);

            // Text.
            auto text = Text2d::CreateText2d("Śliwka", "RetroSerif",
                                             cursorPos, 0.0f, RETRO_PIXEL_SCALE.y * 16.0f,
                                             Color::White, (int)TextStyleFlags::Gradient | (int)TextStyleFlags::Shadow | (int)TextStyleFlags::HalfHeight,
                                             1, AlignMode::BottomLeft);
            renderer.SubmitText2d(text);
            //auto text2 = Text2d::CreateText2d("Have you seen a little girl?", "SmoothSerif",
            //                                  Vector2(10.0f, 20.0f), 0/*DEG_TO_RAD(45.0f)*/, 1.0f / 14.0f,
            //                                  Color::White, (int)TextStyleFlags::Gradient | (int)TextStyleFlags::Shadow,
            //                                  1, AlignMode::CenterLeft);
            //renderer.SubmitText2d(text2);

            //return;

            // GUI button test.
            /*static auto but = Button(Vector2(25.0f, 25.0f), Vector2(25.0f, 25.0f), ScaleMode::VerticalEdge,
                                     []() { Debug::Log("Entering!"); },
                                     [&]()
                                     {
                                        auto& renderer2 = g_App.GetRenderer();
                                        auto  res    = renderer.GetViewportResolution().ToVector2();
                                        float aspect = res.x / res.y;
                                        auto aspectCorrection = Vector2::One;
                                        if (aspect >= 1.0f)
                                        {
                                            aspectCorrection.x = 1.0f / aspect;
                                        }
                                        else
                                        {
                                            aspectCorrection.y = 1.0f / (1.0f / aspect);
                                        }
                                     
                                        auto quad = Shape2d::CreateQuad(Vector2(0.0f,  00.0f),
                                                                            Vector2(0.0f,  50.0f),
                                                                            Vector2(50.0f, 50.0f) * aspectCorrection,
                                                                            Vector2(50.0f, 0.0f) * aspectCorrection,
                                                                            Color(0.0f, 1.0f, 0.2f, 0.4f),
                                                                            Color(0.0f, 1.0f, 0.2f, 0.4f),
                                                                            Color(0.0f, 1.0f, 0.2f, 0.4f),
                                                                            Color(0.0f, 1.0f, 0.2f, 0.4f),
                                                                            0, ScaleMode::VerticalEdge, BlendMode::Alpha);
                                        renderer.SubmitShape2d(quad);
                                     },
                                     []() { Debug::Log("Leaving!"); },
                                     [&]()
                                     {
                                        auto& renderer2 = g_App.GetRenderer();
                                        auto  res    = renderer.GetViewportResolution().ToVector2();
                                        float aspect = res.x / res.y;
                                        auto aspectCorrection = Vector2::One;
                                        if (aspect >= 1.0f)
                                        {
                                            aspectCorrection.x = 1.0f / aspect;
                                        }
                                        else
                                        {
                                            aspectCorrection.y = 1.0f / (1.0f / aspect);
                                        }
                                     
                                        auto quad = Shape2d::CreateQuad(Vector2(0.0f,  00.0f),
                                                                            Vector2(0.0f,  50.0f),
                                                                            Vector2(50.0f, 50.0f) * aspectCorrection,
                                                                            Vector2(50.0f, 0.0f) * aspectCorrection,
                                                                            Color(1.0f, 0.0f, 0.4f, 0.4f),
                                                                            Color(1.0f, 0.0f, 0.4f, 0.4f),
                                                                            Color(1.0f, 0.0f, 0.4f, 0.4f),
                                                                            Color(1.0f, 0.0f, 0.4f, 0.4f),
                                                                            0, ScaleMode::VerticalEdge, BlendMode::Alpha);
                                        renderer.SubmitShape2d(quad);
                                     },
                                     []() { Debug::Log("Clicking!"); },
                                     []() { Debug::Log("Holding!"); },
                                     []() { Debug::Log("Releasing!"); });

            // Check if debug menu is enabled.
            if (!Debug::g_Work.EnableDebugMenu)
            {
                but.Update(input.GetCursorPosition());
            }*/

            // @temp
            /*auto tri0 = Shape2d::CreateTriangle(Vector2(0.0f + 0.2f, 0.5f + 0.2f),
                                                    Vector2(-0.5f + 0.2f, -0.5f + 0.2f),
                                                    Vector2(0.5f + 0.2f, -0.5f + 0.2f),
                                                    Color(1.0f, 0.0f, 1.0f, 0.5f),
                                                    Color(1.0f, 1.0f, 1.0f, 0.5f),
                                                    Color(1.0f, 0.0f, 1.0f, 0.5f),
                                                    0);
            auto tri1 = Shape2d::CreateTriangle(Vector2(0.2f, 0.25f),
                                                    Vector2(-0.25f, -0.25f),
                                                    Vector2(0.25f, -0.25f),
                                                    Color(1.0f, 0.0f, 0.0f, 0.75f),
                                                    Color(0.0f, 1.0f, 1.0f, 0.75f),
                                                    Color(0.0f, 0.0f, 1.0f, 0.75f),
                                                    0);
            auto quad = Shape2d::CreateQuad(Vector2(40.0f, 40.0f),
                                                Vector2(50.0f, 40.0f),
                                                Vector2(50.0f, 50.0f),
                                                Vector2(40.0f, 50.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                Color(0.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                0, ScaleMode::VerticalEdge, BlendMode::Alpha);
            auto line0 = Shape2d::CreateLine(Vector2i(10, 10),
                                                Vector2i(50, 10),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                0);
            auto line1 = Shape2d::CreateLine(Vector2i(15, 11),
                                                Vector2i(70, 11),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                0);
            auto line2 = Shape2d::CreateLine(Vector2i(0, 1),
                                                 Vector2i(0, 239),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 0);
            auto line3 = Shape2d::CreateLine(Vector2i(319, 0),
                                                 Vector2i(1, 0),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 0);
            //renderer.SubmitShape2d(tri0);
            //renderer.SubmitShape2d(tri1);
            renderer.SubmitShape2d(quad);
            renderer.SubmitShape2d(line0);
            renderer.SubmitShape2d(line1);
            //renderer.SubmitShape2d(line2);
            renderer.SubmitShape2d(line3);*/

            // Bullets
            for (int i = 0; i < 9; i++)
            {
                if (i == 0)
                {
                    auto line3 = Shape2d::CreateLine(Vector2i(39,  82 + (i * 16)),
                                                         Vector2i(200, 82 + (i * 16)),
                                                         Color::From8Bit(176, 176, 176),
                                                         Color::From8Bit(160, 128, 64),
                                                         0);
                    renderer.SubmitShape2d(line3);
                }

                auto quadB0 = Shape2d::CreateQuad(
                                                    Vector2i(52, 69 + (i * 16)),
                                                    Vector2i(52, 81 + (i * 16)),
                                                    Vector2i(40, 69 + (i * 16)),
                                                    Vector2i(40, 81 + (i * 16)),
                                                    Color::From8Bit(255, 255, 255),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(255, 255, 255),
                                                    0);
                auto quadB1 = Shape2d::CreateQuad(
                                                    Vector2i(39, 68 + (i * 16)),
                                                    Vector2i(39, 82 + (i * 16)),
                                                    Vector2i(53, 68 + (i * 16)),
                                                    Vector2i(53, 82 + (i * 16)),
                                                    Color::From8Bit(255, 255, 255),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(64,  64,  64),
                                                    1);
                renderer.SubmitShape2d(quadB1);
                renderer.SubmitShape2d(quadB0);
            }

            //auto tri = Shape2d::CreateTriangle(
            //                                       Vector2i(8, 84),
            //                                       Vector2i(16, 76),
            //                                       Vector2i(16, 92),
            //                                       Color::From8Bit(48, 255, 255),
            //                                       Color::From8Bit(48, 255, 128),
            //                                       Color::From8Bit(48, 255, 128),
            //                                       0);
            //renderer.SubmitShape2d(tri);
        }
    }
}

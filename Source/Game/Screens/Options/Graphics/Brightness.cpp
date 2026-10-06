#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Graphics/Brightness.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/SelectionGraphics.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Options.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Game
{
    static void SubmitLines(int brightness)
    {
        constexpr int LINE_COUNT = 20;

        auto& renderer = g_App.GetRenderer();

        // Submit vertical lines.
        for (int i = -(LINE_COUNT / 2); i <= (LINE_COUNT / 2); i++)
        {
            // Compute start and end points.
            auto from = Vector2i((SCREEN_WIDTH / 2) + ((SCREEN_WIDTH - 64) / 20) * i, (SCREEN_HEIGHT / 2) - 20);
            auto to   = Vector2i(from.x, 192);

            // Compute color.
            uchar colorComp = (brightness * 8) + 4;
            auto  color     = Color::From8Bit(colorComp, colorComp, colorComp);

            // Submit line.
            auto line = Shape2d::CreateLine(from, to, color, color);
            renderer.SubmitShape2d(line);
        }
    }

    void Options_ControlBrightnessMenu()
    {
        constexpr auto PROMPT_STR_POS = Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6);
        constexpr auto ENTRY_STR_POS  = Vector2i(SCREEN_WIDTH / 4, (SCREEN_HEIGHT / 12) * 11);
        constexpr auto CONFIG_STR_POS = Vector2i((SCREEN_WIDTH / 3) * 2, (SCREEN_HEIGHT / 12) * 11);
        constexpr auto ARROW_OFFSET   = Vector2i(SCREEN_WIDTH / 16, 0);

        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();
        auto&       options    = g_App.GetOptions();

        bool isLeftHeld  = input.GetAction(In::Left).IsHeld(0.0f, GUI_PULSE_STATE_MIN);
        bool isRightHeld = input.GetAction(In::Right).IsHeld(0.0f, GUI_PULSE_STATE_MIN);

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[2])
        {
            case 0:
            {
                Game_StateStepIncrement(2);
                break;
            }
            case 1:
            {
                ScreenFade_Start(true, true, false);
                Game_StateStepIncrement(2);
                break;
            }
            case 2:
            {
                if (!isLeftHeld || !isRightHeld)
                {
                    if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                    {
                        if (options->BrightnessLevel > 0)
                        {
                            //Sd_SfxPlay(Sfx_Back, 0, Q8(0.25f));
                            options->BrightnessLevel--;
                        }
                    }
                    if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                    {
                        if (options->BrightnessLevel < BRIGHTNESS_LEVEL_MAX)
                        {
                            //Sd_SfxPlay(Sfx_Back, 0, Q8(0.25f));
                            options->BrightnessLevel++;
                        }
                    }
                }

                // Fade screen and leave menu.
                if (input.GetAction(In::Enter).IsClicked() ||
                    input.GetAction(In::Cancel).IsClicked())
                {
                    if (input.GetAction(In::Enter).IsClicked())
                    {
                        //Sd_SfxPlay(Sfx_Confirm, 0, Q8_CLAMPED(0.25f));
                    }
                    else
                    {
                        //Sd_SfxPlay(Sfx_Cancel, 0, Q8_CLAMPED(0.25f));
                    }

                    ScreenFade_Start(true, false, false);
                    Game_StateStepIncrement(2);
                }
                break;
            }
            case 3:
            {
                // Switch to previous menu.
                if (ScreenFade_IsFinished())
                {
                    g_GameWork.background2dColor = { 0, 0, 0 };

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveBrightness);
                }
                break;
            }
        }

        // Set global string color.
        Gfx_StringColorSet(StringColorId_White);

        // Submit text prompt.
        Gfx_StringPositionSet(PROMPT_STR_POS.x, PROMPT_STR_POS.y);
        Gfx_StringDraw(translator(KEY_BRIGHT_MENU_PROMPT));

        // Submit vertical lines.
        SubmitLines(options->BrightnessLevel);

        // Submit entry string.
        Gfx_StringPositionSet(ENTRY_STR_POS.x, ENTRY_STR_POS.y);
        Gfx_StringDraw(translator(KEY_BRIGHT_MENU_LEVEL));

        // Submit config string.
        Gfx_StringPositionSet(CONFIG_STR_POS.x, CONFIG_STR_POS.y);
        Gfx_StringDraw("{M}" + std::to_string(options->BrightnessLevel));

        // Submit arrows.
        Options_DrawArrow(CONFIG_STR_POS - ARROW_OFFSET, SelectionArrowType::Left,  isLeftHeld  && !isRightHeld);
        Options_DrawArrow(CONFIG_STR_POS + ARROW_OFFSET, SelectionArrowType::Right, isRightHeld && !isLeftHeld);
    }
}

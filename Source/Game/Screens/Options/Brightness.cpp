#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Brightness.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/SelectionGraphics.h"
#include "Input/Input.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Utils;

namespace Silent::Game
{
    void Options_BrightnessMenu_Control()
    {
        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();

        // @todo
        // Handle menu state.
        switch (g_GameWork.gameStateSteps[1])
        {
            case BrightnessMenuState_Enter:
                g_GameWork.gameStateSteps[1] = BrightnessMenuState_StartFade;
                g_GameWork.gameStateSteps[2] = 0;
                break;

            case BrightnessMenuState_StartFade:
                ScreenFade_Start(true, true, false);
                g_GameWork.gameStateSteps[1] = BrightnessMenuState_SetConfig;
                g_GameWork.gameStateSteps[2] = 0;
                break;

            case BrightnessMenuState_SetConfig:
                if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    if (g_GameWork.config.brightness != 0)
                    {
                        g_GameWork.config.brightness--;
                        //Sd_SfxPlay(Sfx_Back, 0, Q8_CLAMPED(0.25f));
                    }
                }
                if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    if (g_GameWork.config.brightness < 7)
                    {
                        g_GameWork.config.brightness++;
                        //Sd_SfxPlay(Sfx_Back, 0, Q8_CLAMPED(0.25f));
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
                    g_GameWork.gameStateSteps[1]++;
                    g_GameWork.gameStateSteps[2] = 0;
                }
                break;

            case BrightnessMenuState_Leave:
                // Switch to previous menu.
                if (ScreenFade_IsFinished())
                {
                    ScreenFade_Start(true, true, false);
                    g_GameWork.gameStateSteps[0]   = OptionsMenuState_LeaveBrightness;
                    g_SysWork.counters_1C[1]       = 0;
                    g_GameWork.gameStateSteps[1]   = 0;
                    g_GameWork.gameStateSteps[2]   = 0;
                    g_GameWork.background2dColor.r = 0;
                    g_GameWork.background2dColor.g = 0;
                    g_GameWork.background2dColor.b = 0;
                }
                break;
        }

        // Submit text prompt.
        Gfx_StringPositionSet(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringDraw(translator(KEY_BRIGHT_MENU_PROMPT));

        //func_8003E5E8(g_GameWork.config.brightness);
        Options_BrightnessMenu_ArrowsDraw();
        Options_BrightnessMenu_ConfigDraw();
    }

    void Options_BrightnessMenu_ConfigDraw()
    {
        const auto& translator = g_App.GetTranslator();

        Gfx_StringColorSet(StringColorId_White);

        Gfx_StringPositionSet(SCREEN_WIDTH / 4, 190);
        Gfx_StringDraw(translator(KEY_BRIGHT_MENU_LEVEL));

        Gfx_StringPositionSet(SCREEN_WIDTH / 2, 190);
        Gfx_StringDrawInt(1, g_GameWork.config.brightness);
    }

    void Options_BrightnessMenu_ArrowsDraw()
    {
        static const auto FRONT_ARROWS = std::vector<s_Triangle2d>
        {
            { { 160 + 8,  120 + 84 }, { 160 + 16, 120 + 76 }, { 160 + 16, 120 + 92 } },
            { { 160 + 64, 120 + 84 }, { 160 + 56, 120 + 76 }, { 160 + 56, 120 + 92 } }
        };

        static const auto BORDER_ARROWS = std::vector<s_Triangle2d>
        {
            { { 160 + 7,  120 + 84 }, { 160 + 17, 120 + 74 }, { 160 + 17, 120 + 94 } },
            { { 160 + 65, 120 + 84 }, { 160 + 55, 120 + 74 }, { 160 + 55, 120 + 94 } }
        };

        const auto& input = g_App.GetInput();

        // Determine UI movement direction.
        int dir = 0;
        if (input.GetAction(In::Left).IsHeld(0.0f, 0.5f))
        {
            dir = 1;
        }
        else if (input.GetAction(In::Right).IsHeld(0.0f, 0.5f))
        {
            dir = 2;
        }
        else
        {
            dir = 0;
        }

        // Draw flashing left/right arrows.
        for (int i = 0; i < FRONT_ARROWS.size(); i++)
        {
            Options_Selection_ArrowDraw(FRONT_ARROWS[i], true);
        }

        // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
        for (int i = dir - 1; i < dir; i++)
        {
            Options_Selection_ArrowDraw(BORDER_ARROWS[i], false);
        }
    }
}

#include "Framework.h"
#include "Game/include/screens/options/brightness.h"

#include "Game/dummy.h"
#include "Game/include/screens/options/selection_graphics.h"
#include "Game/include/screens/options/options.h"

namespace Silent::Game
{
    void Options_BrightnessMenu_Control(void)
    {
        // Handle menu state.
        switch (g_GameWork.gameStateStep_598[1])
        {
            case BrightnessMenuState_0:
                // Entry.
                g_GameWork.gameStateStep_598[1] = BrightnessMenuState_1;
                g_GameWork.gameStateStep_598[2] = 0;
                break;

            case BrightnessMenuState_1:
                // Set fade.
                //ScreenFade_Start(true, true, false);
                g_GameWork.gameStateStep_598[1] = BrightnessMenuState_2;
                g_GameWork.gameStateStep_598[2] = 0;
                break;

            case BrightnessMenuState_2:
                // Set config.
                if (g_Controller0.btnsPulsed_18 & ControllerFlag_LStickLeft)
                {
                    if (g_GameWork.config_0.optBrightness_22 != 0)
                    {
                        g_GameWork.config_0.optBrightness_22--;
                        //Sd_PlaySfx(Sfx_Back, 0, Q8_CLAMPED(0.25f));
                    }
                }
                if (g_Controller0.btnsPulsed_18 & ControllerFlag_LStickRight)
                {
                    if (g_GameWork.config_0.optBrightness_22 < 7)
                    {
                        g_GameWork.config_0.optBrightness_22++;
                        //Sd_PlaySfx(Sfx_Back, 0, Q8_CLAMPED(0.25f));
                    }
                }

                // Fade screen and leave menu.
                if (g_Controller0.btnsClicked_10 & (g_GameWork.config_0.controllerConfig_0.enter_0 |
                                                    g_GameWork.config_0.controllerConfig_0.cancel_2))
                {
                    if (g_Controller0.btnsClicked_10 & g_GameWork.config_0.controllerConfig_0.enter_0)
                    {
                        //Sd_PlaySfx(Sfx_Confirm, 0, Q8_CLAMPED(0.25f));
                    }
                    else
                    {
                        //Sd_PlaySfx(Sfx_Cancel, 0, Q8_CLAMPED(0.25f));
                    }

                    //ScreenFade_Start(true, false, false);
                    g_GameWork.gameStateStep_598[1]++;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;

            case BrightnessMenuState_Leave:
                // Switch to previous menu.
                // TODO: Odd check for `ScreenFade_IsFinished()`.
                if (g_Screen_FadeStatus & (1 << 2) && !(g_Screen_FadeStatus & (1 << 1)) && g_Screen_FadeStatus & (1 << 0))
                {
                    //ScreenFade_Start(true, true, false);
                    g_GameWork.gameStateStep_598[0]    = OptionsMenuState_LeaveBrightness;
                    g_SysWork.timer_20                 = 0;
                    g_GameWork.gameStateStep_598[1]    = 0;
                    g_GameWork.gameStateStep_598[2]    = 0;
                    g_GameWork.background2dColor_R_58C = 0;
                    g_GameWork.background2dColor_G_58D = 0;
                    g_GameWork.background2dColor_B_58E = 0;
                }
                break;
        }

        // Draw graphics.
        if (g_GameWork.gameStatePrev_590 == GameState_MainMenu)
        {
            //Gfx_BackgroundSpriteDraw(&g_BrightnessScreenImg0);
        }
        else
        {
            //Gfx_BackgroundSpriteDraw(&g_BrightnessScreenImg1);
        }

        //func_8003E5E8(g_GameWork.config_0.optBrightness_22);
        Options_BrightnessMenu_ArrowsDraw();
        Options_BrightnessMenu_ConfigDraw();
    }

    void Options_BrightnessMenu_ConfigDraw(void)
    {
        //Gfx_StringSetColor(StringColorId_White);
        //Gfx_StringSetPosition(SCREEN_POSITION_X(25.0f), SCREEN_POSITION_Y(79.5f));
        //Gfx_StringDraw("LEVEL_________", 20);
        //Gfx_StringDrawInt(1, g_GameWork.config_0.optBrightness_22);
    }

    void Options_BrightnessMenu_ArrowsDraw(void)
    {
        static const auto FRONT_ARROWS = std::vector<s_Triangle2d>
        {
            { { 8, 84  }, { 16, 76 }, { 16, 92 } },
            { { 64, 84 }, { 56, 76 }, { 56, 92 } }
        };

        static const auto BORDER_ARROWS = std::vector<s_Triangle2d>
        {
            { { 7, 84  }, { 17, 74 }, { 17, 94 } },
            { { 65, 84 }, { 55, 74 }, { 55, 94 } }
        };

        // Determine UI movement direction.
        int btnInput = g_Controller0.btnsHeld_C;
        int dir      = 0;
        if (btnInput & ControllerFlag_LStickLeft)
        {
            dir = 1;
        }
        else if (btnInput & ControllerFlag_LStickRight)
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
            //Options_Selection_ArrowDraw(FRONT_ARROWS[i], true, false);
        }

        // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
        for (int i = dir - 1; i < dir; i++)
        {
            //Options_Selection_ArrowDraw(BORDER_ARROWS[i], false, false);
        }
    }
}

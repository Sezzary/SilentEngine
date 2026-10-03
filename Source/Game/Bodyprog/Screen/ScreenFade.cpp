#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Main/FsQueue.h"
#include "Renderer/Renderer.h"

using namespace Silent::Renderer;

namespace Silent::Game
{
    int    g_Screen_FadeStatus  = 0;
    q19_12 g_ScreenFadeTimestep = Q12(0.0f);
    q19_12 g_ScreenFadeProgress = Q12(0.0f);

    static q19_12 PrevScreenFadeProgress = Q12(0.0f);
    static bool   IsWhite                = false;

    void ScreenFade_Start(bool reset, bool fadeIn, bool isWhite, q19_12 timestep)
    {
        g_Screen_FadeStatus = (((reset) ? ScreenFadeState_FadeOutStart : ScreenFadeState_FadeOutSteps) +
                               ((fadeIn) ? (1 << 2) : 0)) |
                              ((isWhite) ? (1 << 3) : 0);
        
        if (timestep != NO_VALUE)
        {
            g_ScreenFadeTimestep = timestep;
        }
    }

    q19_12 Screen_FadeInProgressGet()
    {
        return Q12(1.0f) - PrevScreenFadeProgress;
    }

    /** @brief Sets the screen fade effect to black or white according to the current status. */
    static void Screen_FadeDrawModeSet()
    {
        IsWhite = IS_SCREEN_FADE_WHITE(g_Screen_FadeStatus);
    }

    void Screen_FadeUpdate()
    {
        auto& renderer = g_App.GetRenderer();

        PrevScreenFadeProgress = g_ScreenFadeProgress;

        q19_12 timestep = Q12(0.0f);
        switch (g_Screen_FadeStatus)
        {
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, true):
            {
                g_ScreenFadeProgress = Q12(0.0f);
                g_Screen_FadeStatus++;
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, true):
            {
                Screen_FadeDrawModeSet();

                if (g_ScreenFadeTimestep > Q12(0.0f))
                {
                    timestep = g_ScreenFadeTimestep;
                }
                else
                {
                    timestep = Q12(3.0f) / (Fs_QueueGetLength() + 1);
                }

                g_ScreenFadeProgress += Q12_MULT_PRECISE(timestep, g_DeltaTimeRaw);
                if (g_ScreenFadeProgress >= Q12_CLAMPED(1.0f))
                {
                    g_ScreenFadeProgress = Q12_CLAMPED(1.0f);
                    g_Screen_FadeStatus++;
                }
                break;
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, true):
            {
                g_ScreenFadeTimestep = Q12(0.0f);
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeInStart, true):
            {
                g_ScreenFadeProgress = Q12_CLAMPED(1.0f);
                g_Screen_FadeStatus++;
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
            {
                Screen_FadeDrawModeSet();
                break;
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeInSteps, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeInSteps, true):
            {
                Screen_FadeDrawModeSet();

                if (g_ScreenFadeTimestep > Q12(0.0f))
                {
                    timestep = g_ScreenFadeTimestep;
                }
                else
                {
                    timestep = Q12(3.0f);
                }

                g_ScreenFadeProgress -= Q12_MULT_PRECISE(timestep, g_DeltaTimeRaw);
                if (g_ScreenFadeProgress <= Q12(0.0f))
                {
                    g_ScreenFadeProgress = Q12(0.0f);
                    ScreenFade_Reset();
                }
                break;
            }
            case SCREEN_FADE_STATUS(ScreenFadeState_Reset, false):
            {
                g_ScreenFadeTimestep = Q12(0.0f);
                g_ScreenFadeProgress = Q12(0.0f);
                g_Screen_FadeStatus  = SCREEN_FADE_STATUS(ScreenFadeState_None, false);
                break;
            }
            default:
            case SCREEN_FADE_STATUS(ScreenFadeState_None, false):
            {
                break;
            }
        }

        renderer.SetLumaFade(Q12_TO_FLT(g_ScreenFadeProgress), IsWhite);
    }
}

#include "Framework.h"
#include "Engine/Game/Test.h"

#include "Engine/Application.h"
#include "Engine/Services/Assets/Assets.h"

namespace Silent
{
    float g_FullscreenAlphaBlend = 0.0f;

    void Entry()
    {
        static bool isInitComplete = false;
        if (isInitComplete)
        {
            // Run main loop.
            //MainLoop();
        }
        else
        {
            auto& time     = g_App.GetTime();
            auto& assets   = g_App.GetAssets();
            auto& renderer = g_App.GetRenderer();

            // Set clear color.
            renderer.SetClearColor(Color::Black);

            // Load `1ST\2ZANKO_E.TIM` ("There are violent and disturbing images...").
            assets.LoadAsset(e_FsFile::FILE_1ST_2ZANKO_E_TIM).wait();

            // Fade in `1ST\2ZANKO_E.TIM` with subtractive blending.
            s32 fade = FP_COLOR(1.0f);
            while (true)
            {
                // Update time after blocking.
                time.Update();

                // Finished fading.
                if (fade < 0)
                {
                    break;
                }

                // Draw fullscreen sprite with subtractive blending.
                // TODO

                fade -= 4;
                g_FullscreenAlphaBlend = (float)fade / 255.0f;

                // Render.
                renderer.Update();

                // Block until next tick.
                time.WaitForNextTick();
            }

            // If files haven't loaded yet, wait until they do.
            while (assets.IsBusy()) // TODO: Create better wait mechanism.
            {
                time.Update();
                time.WaitForNextTick();
            }

            // Load `1ST\FONT8NOC.TIM` (8x8 font).
            assets.LoadAsset(e_FsFile::FILE_1ST_FONT8NOC_TIM).wait();

            // Fade image back out?
            //Gfx_BackgroundSpriteDraw(&g_MainImg0);

            // Cleanup.
            isInitComplete = true;
        }
    }
}

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
            static bool loadAssets = true;

            auto& time     = g_App.GetTime();
            auto& assets   = g_App.GetAssets();
            auto& renderer = g_App.GetRenderer();

            // Set clear color.
            renderer.SetClearColor(Color::Black);

            // Load `1ST\2ZANKO_E.TIM` ("There are violent and disturbing images...").
            if (loadAssets)
            {
                assets.LoadAsset(e_FsFile::FILE_1ST_2ZANKO_E_TIM).wait();
            }

            // Fade in `1ST\2ZANKO_E.TIM` with subtractive blending.
            static s32 fade = FP_COLOR(1.0f);
            if (fade < 0)
            {
                isInitComplete = true;
            }
            else
            {
                // TODO: Submit fullscreen sprite with subtractive blending.

                g_FullscreenAlphaBlend = ((float)fade / FP_COLOR(1.0f));
                fade -= 4;
            }

            // Load `1ST\FONT8NOC.TIM` (8x8 font).
            if (loadAssets)
            {
                assets.LoadAsset(e_FsFile::FILE_1ST_FONT8NOC_TIM).wait();
            }

            loadAssets = false;
        }
    }
}

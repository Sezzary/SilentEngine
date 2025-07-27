#include "Framework.h"
#include "Engine/Game/main.h"

#include "Engine/Application.h"
#include "Engine/Services/Assets/Assets.h"

namespace Silent
{
    float g_FullscreenAlphaBlend = 0.0f;

    void Entry()
    {
        constexpr int FADE_STEP = 4;

        static bool isInitComplete = false;
        if (isInitComplete)
        {
            // Run main loop.
            //MainLoop();
        }
        else
        {
            auto& assets   = g_App.GetAssets();
            auto& renderer = g_App.GetRenderer();

            // Set clear color.
            renderer.SetClearColor(Color::Black);

            // Load `1ST/2ZANKO_E.TIM` ("There are violent and disturbing images...").
            if (assets.GetAsset(e_FsFile::FILE_1ST_2ZANKO_E_TIM)->State == AssetState::Unloaded)
            {
                assets.LoadAsset(e_FsFile::FILE_1ST_2ZANKO_E_TIM).wait();
            }

            // Fade in `1ST/2ZANKO_E.TIM` with luma-multiply blending.
            static s32 fade = FP_COLOR(1.0f);
            if (fade < 0)
            {
                isInitComplete = true;
            }
            else
            {
                // TODO: Submit fullscreen sprite with luma-multiply blending.
                auto color = Color(1.0f, 1.0f, 1.0f, (float)fade / FP_COLOR(1.0f));
                renderer.SubmitScreenSprite(e_FsFile::FILE_1ST_2ZANKO_E_TIM, Vector2(50.0f), FP_DEGREE(0.0f), Vector2::One, color,
                                            0, ScreenSpriteAlignMode::Center, ScreenSpriteScaleMode::Fit, BlendMode::LumaMultiply);

                g_FullscreenAlphaBlend = (float)fade / FP_COLOR(1.0f);
                fade -= FADE_STEP;
            }

            // Load `1ST/FONT8NOC.TIM` (8x8 font).
            if (assets.GetAsset(e_FsFile::FILE_1ST_FONT8NOC_TIM)->State == AssetState::Unloaded)
            {
                assets.LoadAsset(e_FsFile::FILE_1ST_FONT8NOC_TIM).wait();
            }
        }
    }
}

#include "Framework.h"
#include "Engine/Game/main.h"

#include "Engine/Application.h"
#include "Engine/Assets/Assets.h"
#include "Engine/Services/Time.h"

namespace Silent
{
    float g_FullscreenAlphaBlend = 0.0f;

    static void MainLoop()
    {
        s32 vBlanks;
        s32 interval;

        static bool isInitComplete = false;

        // Run game loop.
        if (isInitComplete)
        {
            const auto& time     = g_App.GetTime();
            auto&       renderer = g_App.GetRenderer();

            //g_MainLoop_FrameCount++;

            // Update input.
            /*Joy_ReadP1();
            Demo_ControllerDataUpdate();
            Joy_ControllerDataUpdate();*/

            /*if (MainLoop_ShouldWarmReset() == 2)
            {
                Game_WarmBoot();
                continue;
            }*/

            //g_ObjectTableIdx = GsGetActiveBuff();

            /*if (g_GameWork.gameState_594 >= GameState_MainLoadScreen && g_GameWork.gameState_594 < GameState_MapEvent)
            {
                GsOUT_PACKET_P = (PACKET*)(TEMP_MEMORY_ADDR + (g_ObjectTableIdx << 17));
            }
            else if (g_GameWork.gameState_594 == GameState_InventoryScreen)
            {
                GsOUT_PACKET_P = (PACKET*)(TEMP_MEMORY_ADDR + (g_ObjectTableIdx * 40000));
            }
            else
            {
                GsOUT_PACKET_P = (PACKET*)(TEMP_MEMORY_ADDR + (g_ObjectTableIdx << 15));
            }*/

            /*GsClearOt(0, 0, &g_ObjectTable0[g_ObjectTableIdx]);
            GsClearOt(0, 0, &g_ObjectTable1[g_ObjectTableIdx]);*/

            //g_SysWork.field_22A0 = 0;

            // Call update function for current GameState.
            //D_800A977C[g_GameWork.gameState_594]();

            /*Demo_Update();
            Demo_GameRandSeedSet();*/

            /*if (MainLoop_ShouldWarmReset() == 2)
            {
                Game_WarmBoot();
                continue;
            }*/

            /*func_8003260C();
            func_8002EB88();
            func_800485D8();*/

            /*if (!func_80045B28())
            {
                Fs_QueueUpdate();
            }*/

            /*func_80089128();
            func_8008D78C(); // Camera update?
            DrawSync(0);*/

            // Handle V sync.
            /*if (g_SysWork.flags_22A4 & (1 << 1))
            {
                vBlanks   = VSync(-1);
                g_VBlanks = vBlanks - g_PrevVBlanks;

                Demo_PresentIntervalUpdate();

                interval      = g_Demo_VideoPresentInterval;
                g_PrevVBlanks = vBlanks;

                if (interval < g_IntervalVBlanks)
                {
                    interval = g_IntervalVBlanks;
                }

                do
                {
                    VSync(0);
                    g_VBlanks++;
                    g_PrevVBlanks++;
                }
                while (g_VBlanks < interval);

                g_UncappedVBlanks = g_VBlanks;
                g_VBlanks         = MIN(g_VBlanks, 4);

                vCount     = g_Demo_VideoPresentInterval * H_BLANKS_PER_TICK;
                vCountCopy = g_UncappedVBlanks * H_BLANKS_PER_TICK;
                g_VBlanks  = g_Demo_VideoPresentInterval;
            }
            else
            {
                if (g_SysWork.sysState_8 != SysState_Gameplay)
                {
                    g_VBlanks     = VSync(-1) - g_PrevVBlanks;
                    g_PrevVBlanks = VSync(-1);
                    VSync(0);
                }
                else
                {
                    if (g_Gfx_ScreenFade != 1)
                    {
                        VSync(0);
                    }

                    g_VBlanks     = VSync(-1) - g_PrevVBlanks;
                    g_PrevVBlanks = VSync(-1);

                    if (g_VBlanks < g_IntervalVBlanks)
                    {
                        do
                        {
                            VSync(0);
                            g_VBlanks++;
                            g_PrevVBlanks++;
                        }
                        while (g_VBlanks < g_IntervalVBlanks);
                    }
                }

                // Update V blanks.
                g_UncappedVBlanks = g_VBlanks;
                g_VBlanks         = MIN(g_VBlanks, V_BLANKS_MAX);
            }*/

            // Update delta time.
            /*g_DeltaTime0 =
            g_DeltaTime1 = FP_FLOAT_TO((1.0f / (float)TICKS_PER_SECOND) * time.GetTicks(), Q12_SHIFT);*/

            /*g_DeltaTime0 = FP_MULTIPLY(vCount, H_BLANKS_FP_TO_SEC_SCALE, Q12_SHIFT);
            g_DeltaTime1 = FP_MULTIPLY(vCountCopy, H_BLANKS_FP_TO_SEC_SCALE, Q12_SHIFT);
            g_DeltaTime2 = FP_MULTIPLY(vCount, H_BLANKS_UNKNOWN_SCALE, Q12_SHIFT); // @todo Unknown time scale.
            GsClearVcount();*/
            
            // Set clear color.
            //renderer.SetClearColor(Color::From8Bit(g_GameWork.background2dColor_R_58C, g_GameWork.background2dColor_G_58D, g_GameWork.background2dColor_B_58E));
            // Then draw everything.
        }
        // Initialize engine.
        else
        {
            /*GsInitVcount();
            Savegame_CardCleanInit();
            func_8002E7BC();
            func_8002E85C();
            Joy_Init();
            VSyncCallback(&Gfx_VSyncCallback);
            InitGeom();
            func_8004BB10(); // Initializes something for graphics.
            func_800890B8();
            sd_init();*/

            isInitComplete = true;
        }
    }

    void Entry()
    {
        constexpr int FADE_STEP = 4;

        static bool isInitComplete = false;

        // Run game.
        if (isInitComplete)
        {
            // Run main loop.
            MainLoop();
        }
        // Initialize.
        else
        {
            auto& assets   = g_App.GetAssets();
            auto& renderer = g_App.GetRenderer();

            // Set clear color.
            renderer.SetClearColor(Color::Black);

            // Load `1ST/2ZANKO_E.TIM` ("There are violent and disturbing images...").
            assets.LoadAsset(e_FsFile::FILE_1ST_2ZANKO_E_TIM).wait();

            // Fade in `1ST/2ZANKO_E.TIM` with luma-multiply blending.
            static int fade = 0;
            if (fade > FP_COLOR(1.0f))
            {
                isInitComplete = true;
            }
            else
            {
                // @todo Non-functioning call. Submit fullscreen sprite with luma-multiply blending.
                auto color = Color(1.0f, 1.0f, 1.0f, (float)fade / (float)FP_COLOR(1.0f));
                renderer.SubmitScreenSprite(e_FsFile::FILE_1ST_2ZANKO_E_TIM, Vector2::Zero, Vector2::One, SCREEN_SPACE_RES / 2.0f, FP_ANGLE(0.0f), Vector2::One,
                                            color, 0, AlignMode::Center, ScaleMode::Fit, BlendMode::Opaque);

                g_FullscreenAlphaBlend = (float)fade / (float)FP_COLOR(1.0f); // TEMP.

                fade += FADE_STEP;
            }

            // Load `1ST/FONT8NOC.TIM` (8x8 font).
            assets.LoadAsset(e_FsFile::FILE_1ST_FONT8NOC_TIM).wait();
        }
    }
}

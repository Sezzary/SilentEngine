#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Stream/Stream.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/FsScreens.h"
#include "Game/Main/FileInfo.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Utils/Video.h"

using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Game
{
    void GameState_MovieIntroFadeIn_Update()
    {
        const auto& input = g_App.GetInput();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
                ScreenFade_Start(true, true, false);
                GameFs_TitleGfxLoad();

                g_GameWork.gameStateSteps[0]++;
                break;

            case 1:
                if (input.HasUserActionInput() || g_SysWork.gameStateCounter > SECONDS_60_FPS(5))
                {
                    ScreenFade_Start(false, false, false);
                    g_GameWork.gameStateSteps[0] = 2;
                }
                break;

            case 2:
                if (ScreenFade_IsFinished())
                {
                    Fs_QueueWaitForEmpty();
                    Game_StateSetNext(GameState_MovieIntro);
                }
                break;
        }
    }

    void GameState_MovieIntro_Update()
    {
        const char* videoName = (g_GameWorkConst->config.extraOptionsEnabled & (1 << 0)) ? "C1_20670.MPG" :
                                                                                           "C2_20670.MPG";
        if (!PlayFmv(std::string(videoName)))
        {
            Game_StateSetNext(GameState_MainMenu);
            g_ScreenFadeTimestep = Q12(1.0f);
        }
    }

    void GameState_MovieOpening_Update()
    {
        if (!PlayFmv("M1_03500.MPG"))
        {
            Game_StateSetNext(GameState_MainLoadScreen);
        }
    }

    void GameState_ExitMovie_Update()
    {
        Game_StateSetNext(GameState_InGame);
    }

    void GameState_DebugMoviePlayer_Update()
    {
        // @stub
    }

    void GameState_MovieIntroAlternate_Update()
    {
        if (!PlayFmv("C1_20670.MPG"))
        {
            Game_StateSetNext(GameState_MainMenu);
            g_ScreenFadeTimestep = Q12(1.0f);
        }
    }

    bool PlayFmv(const std::string& name)
    {
        const auto& input    = g_App.GetInput();
        auto&       renderer = g_App.GetRenderer();
        auto&       video    = g_App.GetVideo();

        // Start playing new video.
        if (!video.IsLoaded() || name != video.GetName())
        {
            video.Play(name);
        }
        // Update active video playback.
        else
        {
            if (!video.IsPlaying() || input.GetAction(In::Enter).IsClicked())
            {
                video.Stop();
                return false;
            }
            else
            {
                video.Update(Q12_TO_FLT(g_DeltaTime));
            }
        }

        // Submit fullscreen video sprite.
        auto  sprite = Sprite2d::CreateSprite2d(video.GetName(), Vector2::Zero, Vector2::One,
                                                SCREEN_SPACE_RES * 0.5f, DEG_TO_RAD(0.0f), 1.0f, video.GetAspectRatio(),
                                                Color::White, NO_VALUE,
                                                DEPTH_2D_MAX, AlignMode::Center, ScaleMode::Fit, BlendMode::Opaque);
        renderer.SubmitSprite2d(sprite);
        return true;
    }
}

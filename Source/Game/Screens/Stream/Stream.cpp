#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Stream/Stream.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/FsScreens.h"
#include "Game/Main/FileInfo.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"
#include "Utils/Video.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Game
{
    void GameState_MovieIntro_Update()
    {
        enum class StateStep
        {
            FadeIn,
            Play
        };

        switch (g_GameWork.gameStateSteps[0])
        {
            case (int)StateStep::FadeIn:
            {
                ScreenFade_Start(false, true, false, Q12(1.0f));

                Game_StateStepIncrement(0);
                break;
            }
            case (int)StateStep::Play:
            {
                if (!PlayFmv("C1_20670.MPG"))
                {
                    Game_StateSetNext(GameState_MainMenu);
                    g_ScreenFadeTimestep = Q12(1.0f);
                }
                break;
            }
        }
    }

    void GameState_MovieOpening_Update()
    {
        enum class StateStep
        {
            ResetFade,
            Play
        };

        switch (g_GameWork.gameStateSteps[0])
        {
            case (int)StateStep::ResetFade:
            {
                ScreenFade_Reset();

                Game_StateStepIncrement(0);
                break;
            }
            case (int)StateStep::Play:
            {
                if (!PlayFmv("M1_03500.MPG"))
                {
                    Game_StateSetNext(GameState_MainLoadScreen);
                }
                break;
            }
        }
    }

    void GameState_ExitMovie_Update()
    {
        Game_StateSetNext(GameState_InGame);
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
            if (input.GetAction(In::Enter).IsClicked()  ||
                input.GetAction(In::Cancel).IsClicked() ||
                !video.IsPlaying())
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

#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/GameMain.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/MemCard.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Events/GameSysStates.h"
#include "Game/Bodyprog/GameBoot/GameLoad.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/BKonami/BKonami.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/SaveLoad/SaveLoad.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Services;

namespace Silent::Game
{
    s32 g_Demo_FrameCount = 0;
    s32 g_WarmBootTimer   = 0;

    static s32 g_PrevVBlanks = 0;

    // Audio task for `SD_Call` meant to load some VAB audio.
    u16 g_BaseVabAudiosTaskId[] =
    {
        160,
        162,
        0
    };

    // @todo Remove checklist when all are working.
    void (*g_GameStateUpdateFuncs[])() =
    {
        GameState_Init_Update,
        GameState_LanguageScreen_Update,
        GameState_KonamiLogo_Update,
        GameState_KcetLogo_Update,
        GameState_AutoLoadSavegame_Update,         // @todo
        GameState_MovieIntroAlternate_Update,
        GameState_MovieIntro_Update,
        GameState_MainMenu_Update,                 // @todo
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_MovieOpening_Update,
        GameState_LoadScreen_Update,               // @todo
        GameState_InGame_Update,                   // @todo
        GameState_MapEvent_Update,                 // @todo
        GameState_ExitMovie_Update,                // @todo
        nullptr,//GameState_ItemScreens_Update,    // @todo
        nullptr,//GameState_PaperMapScreen_Update, // @todo
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_DebugMoviePlayer_Update,         // @todo
        GameState_Options_Update,                  // @todo
        GameState_LoadStatusScreen_Update,         // @todo
        GameState_LoadMapScreen_Update,            // @todo
        nullptr,//GameState_Credits_Update         // @todo
    };

    void GameState_Init_Update()
    {
        const auto& input      = g_App.GetInput();
        const auto& options    = g_App.GetOptions();
        const auto& translator = g_App.GetTranslator();
        auto&       assets     = g_App.GetAssets();
        auto&       renderer   = g_App.GetRenderer();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
            {
                assets.Load("Textures/SplashScreen.png");

                Game_StateStepIncrement(0);
                return;
            }
            case 1:
            {
                if (assets["Textures/SplashScreen.png"]->State != AssetState::Loaded)
                {
                    return;
                }

                ScreenFade_Start(true, true, false, Q12(1.0f));

                Game_StateStepIncrement(0);
                return;
            }
            case 2:
            {
                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                Game_StateStepIncrement(0);
                break;
            }
            case 3:
            {
                //SD_Init();

                //if (!Sd_AudioStreamingCheck())
                {
                    int vabAudioTaskId = g_BaseVabAudiosTaskId[g_GameWork.gameStateSteps[1]];
                    if (vabAudioTaskId != 0)
                    {
                        SD_Call(vabAudioTaskId);
                        Game_StateStepIncrement(1);
                    }
                    else
                    {
                        Game_StateStepIncrement(0);
                    }
                }
                break;
            }
            case 4:
            {
                assets.Load("Psx/1ST/KONAMI.TIM");

                Game_StateStepIncrement(0);
                break;
            }
            case 5:
            {
                if (input.GetAction(In::Enter).IsClicked()  ||
                    input.GetAction(In::Cancel).IsClicked() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(5.0f))
                {
                    ScreenFade_Start(false, false, false, Q12(1.0f));

                    Game_StateStepIncrement(0);
                }
                break;
            }
            case 6:
            {
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    Game_StateSetNext(GameState_LanguageScreen);
                    //Game_StateSetNext(GameState_KonamiLogo);

                    // @todo
                    //if (options.HasCreatedNewFile())
                    //{
                    //    Game_StateSetNext(GameState_LanguageScreen);
                    //}
                    //else
                    //{
                    //    Game_StateSetNext(GameState_SplashScreen);
                    //}
                }
                break;
            }
        }

        Screen_BackgroundImgDraw("Textures/SplashScreen.png", true, NO_VALUE);
        
        // @todo Style it properly.
        Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 7) * 3);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringDraw(translator(KEY_INTRO_WARNING));
    }
}

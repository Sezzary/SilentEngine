#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/GameMain.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
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
#include "Game/Screens/Boot/Boot.h"
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
        GameState_SplashScreen_Update,
        GameState_LogosScreen_Update,
        GameState_MovieIntro_Update,
        GameState_AutoLoadSavegame_Update,         // @todo
        GameState_MainMenu_Update,
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_MovieOpening_Update,
        GameState_LoadScreen_Update,               // @todo
        GameState_InGame_Update,                   // @todo
        GameState_MapEvent_Update,                 // @todo
        GameState_ExitMovie_Update,                // @todo
        nullptr,//GameState_ItemScreens_Update,    // @todo
        nullptr,//GameState_PaperMapScreen_Update, // @todo
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_Options_Update,                  // @todo
        GameState_LoadStatusScreen_Update,         // @todo
        GameState_LoadMapScreen_Update,            // @todo
        nullptr,//GameState_Credits_Update         // @todo
    };
}

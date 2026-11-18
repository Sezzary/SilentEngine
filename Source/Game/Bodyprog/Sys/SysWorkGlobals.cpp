#include "Framework.h"
#include "Psx.h"

#include "Game/Bodyprog/Bodyprog.h"

namespace Silent::Game
{
    s_SysWork  g_SysWork;
    s_GameWork g_GameWork;

    s_GameWork* const       g_GameWorkConst = &g_GameWork;
    s_Savegame* const       g_SavegamePtr   = &g_GameWork.savegame;
    s_ControllerData* const g_Controller0   = &g_GameWork.controllers[0];
    s_ControllerData* const g_Controller1   = &g_GameWork.controllers[1];
    s_GameWork* const       g_GameWorkPtr   = &g_GameWork;
}

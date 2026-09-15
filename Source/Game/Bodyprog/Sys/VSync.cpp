#include "Framework.h"
#include "Psx.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Sys/GameMain.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"

namespace Silent::Game
{
    void Screen_VSyncCallback() // 0x80032B80
    {
        //g_Demo_FrameCount++; // `extern` from demo.h
        g_WarmBootTimer++;

        g_SysWork.gameStateCounter++;
        g_SysWork.gameStateStepCounter++;
        g_SysWork.sysStateCounter++;
    }
}

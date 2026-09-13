#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/FsScreens.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    void GameFs_TitleGfxSeek()
    {
        Fs_QueueStartSeek(FILE_TIM_TITLE_E_TIM);
    }

    void GameFs_TitleGfxLoad()
    {
        Fs_QueueStartReadTim(FILE_TIM_TITLE_E_TIM);
    }

    void GameFs_StreamBinSeek()
    {
        // @stub
    }

    void GameFs_StreamBinLoad()
    {
        // @stub
    }

    void GameFs_OptionBinLoad()
    {
        Fs_QueueStartReadTim(FILE_TIM_OPTION_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    }

    void GameFs_SaveLoadBinLoad()
    {
        Fs_QueueStartReadTim(FILE_TIM_SAVELOAD_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    }

    void func_80032CE8()
    {
        Gfx_StringPositionSet(108, 104);
        Gfx_StringDraw("{7}Now loading.");
    }
}

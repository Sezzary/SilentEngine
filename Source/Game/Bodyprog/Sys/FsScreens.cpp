#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/FsScreens.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    void GameFs_TitleGfxSeek() // 0x80032bd0
    {
        Fs_QueueStartSeek(FILE_TIM_TITLE_E_TIM);
    }

    void GameFs_TitleGfxLoad() // 0x80032bf0
    {
        Fs_QueueStartReadTim(FILE_TIM_TITLE_E_TIM, FS_BUFFER_3, &g_TitleImg);
    }

    void GameFs_StreamBinSeek() // 0x80032C20
    {
        // @stub
    }

    void GameFs_StreamBinLoad() // 0x80032C40
    {
        // @stub
    }

    void GameFs_OptionBinLoad() // 0x80032C68
    {
        Fs_QueueStartReadTim(FILE_TIM_OPTION_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    }

    void GameFs_SaveLoadBinLoad() // 0x80032CA8
    {
        Fs_QueueStartReadTim(FILE_TIM_SAVELOAD_TIM, FS_BUFFER_1, &g_ItemInspectionImg);
    }

    void func_80032CE8() // 0x80032CE8
    {
        Gfx_StringPositionSet(108, 104);
        Gfx_StringDraw("{7}Now loading.");
    }
}

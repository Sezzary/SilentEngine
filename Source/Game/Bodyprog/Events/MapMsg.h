#pragma once

#include "Game/Bodyprog/Text/TextDraw.h"

namespace Silent::Game
{
    constexpr int DEFAULT_MAP_MESSAGE_LENGTH     = 99;
    constexpr int MAP_MESSAGE_DISPLAY_ALL_LENGTH = 400; /** Long string length is used to display a whole message instantly without a rollout. */

    /** @brief Map message indices.
     *
     * @note Each map has its own messages, with the first 15 hardcoded to be the same.
     */
    enum e_MapMsgIdx
    {
        MapMsgIdx_Yes               = 0,
        MapMsgIdx_No                = 1,
        MapMsgIdx_SaveGame          = 2, /** "Someday, someone may experience..." */
        MapMsgIdx_NoMap             = 3,
        MapMsgIdx_TooDarkForMap     = 4,
        MapMsgIdx_FirstAidSelect    = 5,
        MapMsgIdx_HealthDrinkSelect = 6,
        MapMsgIdx_AmpouleSelect     = 7,
        MapMsgIdx_HandgunAmmoSelect = 8,
        MapMsgIdx_RifleAmmoSelect   = 9,
        MapMsgIdx_ShotgunAmmoSelect = 10,
        MapMsgIdx_DoorJammed        = 11,
        MapMsgIdx_DoorLocked        = 12,
        MapMsgIdx_DoorUnlocked      = 13,
        MapMsgIdx_NowMaking         = 14  /** @unused? */
    };

    /** @brief Map message states. Returned by `MapMsg_Draw`. */
    enum e_MapMsgState
    {
        MapMsgState_Finish       = NO_VALUE, /** Initial setup complete, cutscene timer complete, or input to continue from user received. */
        MapMsgState_Idle         = 0,        /** Continue displaying message. */
        MapMsgState_SelectEntry1 = 1,        /** First entry selected in selection dialog. */
        MapMsgState_SelectEntry2 = 2,        /** Second entry selected in selection dialog. */
        MapMsgState_SelectEntry3 = 3         /** Third entry selected in selection dialog. */
    };

    /** @brief Map message audio types. */
    enum e_MapMsgAudioType
    {
        MapMsgAudioType_None                 = 0,
        MapMsgAudioType_VoiceClipUnskippable = 1, /** Set by `{J0}`. For unskippable cutscenes with individual audio files per message page. */
        MapMsgAudioType_VoiceClipSkippable   = 2, /** Set by `{J1}`. For skippable cutscenes with individual audio files per message page. */
        MapMsgAudioType_VoiceStream          = 3  /** Set by `{J2}`. For unskippable cutscenes with a single audio file (e.g. video tape cutscene). */
    };

    struct s_MapMsgSelect
    {
        int maxIdx;
        int selectedEntryIdx;
    };

    int Gfx_MapMsg_Draw(int mapMsgIdx);

    MsgSelectArg Gfx_MapMsg_SelectionUpdate(int mapMsgIdx, int* displayLength);

    void func_80037124();
}

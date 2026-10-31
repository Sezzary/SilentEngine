#include "Framework.h"
#include "Game/Maps/Map0S00/MapEvents.h"

#include "Game/Bodyprog/EventFlags.h"
#include "Game/Bodyprog/Map/Map.h"
#include "Game/Bodyprog/Sound/Sfx.h"
#include "Game/Game.h"

namespace Silent::Game
{
    s_EventData MAP_EVENTS[] =
    {
        // [0]
        {
            .completeFlag = EventFlag_1,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_EventCallback,
            .eventParam        = 2, // `MapEvent_CutsceneOpening`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [1]
        {
            .completeFlag = EventFlag_2,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 7,
            .sysState          = SysState_EventCallback,
            .eventParam        = 3, // `MapEvent_CutsceneCherylFootsteps0`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [2]
        {
            .completeFlag = EventFlag_2,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 8,
            .sysState          = SysState_EventCallback,
            .eventParam        = 4, // `MapEvent_CutsceneCherylFootsteps1`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [3]
        {
            .completeFlag = EventFlag_2,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 9,
            .sysState          = SysState_EventCallback,
            .eventParam        = 5, // `MapEvent_CutsceneCherylFootsteps2`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [4]
        {
            .completeFlag = EventFlag_2,
            .triggerType       = MapTriggerType_TouchObbFacing,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 5,
            .sysState          = SysState_EventCallback,
            .eventParam        = 5, // `MapEvent_CutsceneCherylFootsteps2`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [5]
        {
            .requiredFlag = EventFlag_2,
            .completeFlag = EventFlag_3,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_Fmv,
            .eventParam        = 16,
        },
        // [6]
        {
            .requiredFlag = EventFlag_3,
            .completeFlag = EventFlag_4,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_EventCallback,
            .eventParam        = 6, // `MapEvent_CutsceneCherylSpotted`
        },
        // [7]
        {
            .requiredFlag = EventFlag_4,
            .completeFlag = EventFlag_12,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 10,
            .sysState          = SysState_EventSetFlag,
        },
        // [8]
        {
            .requiredFlag = EventFlag_12,
            .completeFlag = EventFlag_13,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 11,
            .sysState          = SysState_EventCallback,
            .eventParam        = 7, // `MapEvent_CutsceneCherylRedirect0`
        },
        // [9]
        {
            .requiredFlag = EventFlag_4,
            .completeFlag = EventFlag_12,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 12,
            .sysState          = SysState_EventCallback,
            .eventParam        = 8, // `MapEvent_CutsceneCherylRedirect1`
        },
        // [10]
        {
            .requiredFlag = EventFlag_4,
            .completeFlag = EventFlag_12,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 13,
            .sysState          = SysState_EventCallback,
            .eventParam        = 9, // `MapEvent_CutsceneCherylRedirect2`
        },
        // [11]
        {
            .requiredFlag = EventFlag_4,
            .completeFlag = EventFlag_12,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 14,
            .sysState          = SysState_EventCallback,
            .eventParam        = 10, // `MapEvent_CutsceneCherylRedirect3`
        },
        // [12]
        {
            .completeFlag = EventFlag_7,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 15,
            .sysState          = SysState_EventCallback,
            .eventParam        = 11, // `MapEvent_CutsceneCherylIntoTheAlley`
        },
        // [13]
        {
            .completeFlag = EventFlag_7,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 16,
            .sysState          = SysState_EventCallback,
            .eventParam        = 11, // `MapEvent_CutsceneCherylIntoTheAlley`
        },
        // [14]
        {
            .completeFlag = EventFlag_7,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 17,
            .sysState          = SysState_EventCallback,
            .eventParam        = 12, // `func_800DB26C`
        },
        // [15]
        {
            .completeFlag = EventFlag_13,
            .triggerType       = MapTriggerType_TouchObbFacing,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 18,
            .sysState          = SysState_LoadRoom,
            .eventParam        = 19,
            .sfxPairIdx        = SfxPairIdx_4,
        },
        // [16]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 19,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 24,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [17]
        {
            .completeFlag = EventFlag_14,
            .triggerType       = MapTriggerType_TouchObbFacing,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 20,
            .sysState          = SysState_LoadRoom,
            .eventParam        = 22,
            .sfxPairIdx        = SfxPairIdx_4,
        },
        // [18]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 22,
            .sysState        = SysState_LoadRoom,
            .eventParam      = 21,
            .sfxPairIdx      = SfxPairIdx_4,
        },
        // [19]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 21,
            .sysState        = SysState_LoadRoom,
            .eventParam      = 22,
            .sfxPairIdx      = SfxPairIdx_4,
        },
        // [20]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 23,
            .sysState        = SysState_EventCallback,
            .eventParam      = 0, // `func_800D9610`
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [21]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 24,
            .sysState        = SysState_EventCallback,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [22]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 25,
            .sysState        = SysState_EventCallback,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [23]
        {
            .requiredFlag = EventFlag_15,
            .completeFlag = EventFlag_17,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_EventCallback,
            .eventParam        = 13, // `MapEven_CutsceneAlleyGetsDarker`
        },
        // [24]
        {
            .requiredFlag = EventFlag_14,
            .completeFlag = EventFlag_19,
            .triggerType       = MapTriggerType_TouchAabb,
            .mapPointIdx       = 27,
            .sysState          = SysState_EventCallback,
            .eventParam        = 14, // `func_800DB870`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [25]
        {
            .requiredFlag = EventFlag_19,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 69,
            .sysState          = SysState_ReadMessage,
            .eventParam        = 26,
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [26]
        {
            .requiredFlag = EventFlag_19,
            .triggerType       = MapTriggerType_TouchObbFacing,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 70,
            .sysState          = SysState_ReadMessage,
            .eventParam        = 26,
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [27]
        {
            .requiredFlag = EventFlag_16,
            .completeFlag = EventFlag_24,
            .triggerType       = MapTriggerType_TouchObb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 26,
            .sysState          = SysState_EventCallback,
            .eventParam        = 15, // `MapEvent_CutsceneAlleyNightmare`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [28]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 28,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 29,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [29]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 29,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 29,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [30]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 30,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 29,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [31]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 31,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 29,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [32]
        {
            .requiredFlag = EventFlag_25,
            .completeFlag = EventFlag_26,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_EventCallback,
            .eventParam        = 16, // `func_800DBE00`
            .transitionFlags   = AreaTransitionFlag_UnfreezeWorld,
        },
        // [33]
        {
            .requiredFlag = EventFlag_26,
            .triggerType       = MapTriggerType_Tick,
            .sysState          = SysState_LoadOverlay,
            .eventParam        = 4,
            .mapIdx            = MapIdx_MAP0_S01,
        },
        // [34]
        {
            .completeFlag = EventFlag_20,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 32,
            .sysState          = SysState_EventSetFlag,
        },
        // [35]
        {
            .completeFlag = EventFlag_21,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 33,
            .sysState          = SysState_EventSetFlag,
        },
        // [36]
        {
            .completeFlag = EventFlag_22,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 34,
            .sysState          = SysState_EventSetFlag,
        },
        // [37]
        {
            .completeFlag = EventFlag_23,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 35,
            .sysState          = SysState_EventSetFlag,
        },
        // [38]
        {
            .triggerType    = MapTriggerType_TouchObbFacing,
            .activationType = MapTriggerActivationType_Button,
            .mapPointIdx    = 36,
            .sysState       = SysState_ReadMessage,
            .eventParam     = 31,
        },
        // [39]
        {
            .triggerType    = MapTriggerType_TouchObbFacing,
            .activationType = MapTriggerActivationType_Button,
            .mapPointIdx    = 37,
            .sysState       = SysState_ReadMessage,
            .eventParam     = 31,
        },
        // [40]
        {
            .requiredFlag = EventFlag_24,
            .triggerType       = MapTriggerType_TouchObbFacing,
            .activationType    = MapTriggerActivationType_Button,
            .mapPointIdx       = 38,
            .sysState          = SysState_ReadMessage,
            .eventParam        = 32,
        },
        // [41]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 39,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 33,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [42]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 40,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 33,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [43]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 41,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 33,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [44]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 42,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [45]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 43,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [46]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 44,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [47]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 45,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [48]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 46,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [49]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 47,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [50]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 48,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [51]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 49,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [52]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 50,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [53]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 51,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 34,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [54]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 52,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 37,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [55]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 53,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 37,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [56]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 54,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 37,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [57]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 55,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [58]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 56,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [59]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 57,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [60]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 58,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [61]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 59,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [62]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 60,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [63]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 61,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [64]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 62,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [65]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 63,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [66]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 64,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [67]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 65,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [68]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 66,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [69]
        {
            .triggerType     = MapTriggerType_TouchObbFacing,
            .activationType  = MapTriggerActivationType_Button,
            .mapPointIdx     = 67,
            .sysState        = SysState_ReadMessage,
            .eventParam      = 35,
            .transitionFlags = AreaTransitionFlag_UnfreezeWorld,
        },
        // [70]
        {
            .completeFlag = EventFlag_MapMark_OldTown_BachmanRdTopCross,
            .triggerType       = MapTriggerType_TouchAabb,
            .activationType    = MapTriggerActivationType_Exclusive,
            .mapPointIdx       = 68,
            .sysState          = SysState_EventSetFlag,
        },
        // [71]
        {
            .triggerType = MapTriggerType_EndOfArray,
        }
    };
}

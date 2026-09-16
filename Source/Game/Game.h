#pragma once

#include "Psx.h"
#include "Game/libkpad.h"

#include "Game/Bodyprog/Anim.h"
#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Events/Bgm.h"
#include "Game/Bodyprog/Formats/Anm.h"
#include "Game/Bodyprog/Items.h"
#include "Game/Bodyprog/Model.h"
#include "Game/Bodyprog/Savegame.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Maps/Characters/Harry.h"

namespace Silent::Game
{
    constexpr int    TICKS_PER_SECOND = 60; /** Game has a variable timestep with 60 ticks max. */
    constexpr q19_12 TIMESTEP_30_FPS  = Q12(1.0f / (float)(TICKS_PER_SECOND / 2));
    constexpr q19_12 TIMESTEP_60_FPS  = Q12(1.0f / (float)(TICKS_PER_SECOND));

    constexpr int SCREEN_WIDTH                   = 320;
    constexpr int SCREEN_HEIGHT                  = 240;
    constexpr int FRAMEBUFFER_HEIGHT_PROGRESSIVE = 224;
    constexpr int FRAMEBUFFER_HEIGHT_INTERLACED  = FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2;
    constexpr int ORDERING_TABLE_SIZE            = 2048;

    /** @brief Converts seconds to frames at 60 FPS.
     *
     * @param sec Seconds to convert.
     * @return Frames at 60 FPS.
     */
    #define SECONDS_60_FPS(sec) \
        (s32)((sec) * 30)

    /** @brief Converts a floating-point Y screen position in percent to a fixed-point Y screen coodinate. */
    #define SCREEN_POSITION_Y(percent) \
        (s32)(SCREEN_HEIGHT * ((percent) / 100.0f))

    /** @brief Packs a weapon attack containing a weapon ID and attack input type.
     *
     * @param weaponId Weapon ID.
     * @param attackInputType Attack input type.
     * @return Packed weapon attack containing a weapon ID and attack input type.
     */
    #define WEAPON_ATTACK(weaponId, attackInputType) \
        ((weaponId) + ((attackInputType) * 10))

    /** @brief Retrieves the weapon ID from a packed weapon attack.
     *
     * @param weaponAttack Packed weapon attack containing a weapon ID and attack input type.
     * @return Weapon ID.
     */
    #define WEAPON_ATTACK_ID_GET(weaponAttack) \
        ((weaponAttack) % 10)

    /** @brief Creates a bitmask with a contiguous range of bits set.
     * For use with `s_PlayerExtra::disabledAnimBones`.
     *
     * Generates an `unsigned int` mask with all bits in the range `[fromInclusive, toInclusive]` set.
     *
     * For example:
     * - `BITMASK_RANGE(0, 2)` -> 0b000...0111 (decimal 7).
     * - `BITMASK_RANGE(4, 11)` -> 0b000...111111110000 (decimal 4080).
     *
     * @param fromInclusive Index of the lowest bit to set (0 = least significant bit).
     * @param toInclusive Index of the highest bit to set.
     * @return Bitmask with the specified range of bits set to 1 (`unsigned int`).
     */
    #define BITMASK_RANGE(fromInclusive, toInclusive) \
        (((~0u << (fromInclusive)) & ~(~0u << ((toInclusive) + 1))))

    // @todo Use this instead.
    /*bool Player_InMapChunkCheck(int x0, int x1, int x2, int x3,
                                int z0, int z1, int z2, int z3)
    {
        const auto& pos = g_SysWork.playerWork.player.position;

        int chunkIdxX = pos.vx / Q12(40.0f); // @todo Use `CHUNK_SIZE`.
        if (!((pos.vx >  Q12(0.0f) && (chunkIdxX + x0) == x1) || 
              (pos.vx <= Q12(0.0f) && (chunkIdxX + x2) == x3)))
        {
            return false;
        }

        int chunkIdxZ = pos.vz / Q12(40.0f); // @todo Use `CHUNK_SIZE`.
        if (!((pos.vz >  Q12(0.0f) && (chunkIdxZ + z0) == z1) || 
              (pos.vz <= Q12(0.0f) && (chunkIdxZ + z2) == z3)))
        {
            return false;
        }
        
        return true;
    }*/

    #define MAP_CHUNK_CHECK_VARIABLE_DECL() \
        s32 __chunkIdx

    /** @hack This macro requires a variable `s32 __chunkIdx` to be declared before using it.
     * The macro `MAP_CHUNK_CHECK_VARIABLE_DECL` declares that variable and must be called before this macro.
     * The first argument is `vx` or `vz`, which is used as the component name in `VECTOR3`.
     * @bug Some maps appear to have a bug where the negative position check will never be true because they check
     * if the chunk index will be a positive number. Seems like they forgot to use `ABS`?
     */
    #define PLAYER_IN_MAP_CHUNK(comp, x0, x1, x2, x3)                                               \
        (__chunkIdx = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                       \
        ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx + (x0)) == (x1)) || \
         (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx + (x2)) == (x3))))

    #define PLAYER_NOT_IN_MAP_CHUNK(comp, x0, x1, x2, x3)                                           \
        (__chunkIdx = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                       \
        ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx + (x0)) != (x1)) || \
         (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx + (x2)) != (x3))))

    #define MAP_CHUNK_CHECK_VARIABLE_DECL_2() \
        s32 __chunkIdx2

    #define PLAYER_IN_MAP_CHUNK_2(comp, x0, x1, x2, x3)                                             \
        (__chunkIdx2 = g_SysWork.playerWork.player.position.comp / Q12(40.0f),                      \
        ((g_SysWork.playerWork.player.position.comp >  Q12(0.0f) && (__chunkIdx2 + (x0)) < (x1)) || \
         (g_SysWork.playerWork.player.position.comp <= Q12(0.0f) && (__chunkIdx2 + (x2)) < (x3))))

    #define PLAYER_NEAR_POS(comp, base, tol)                                                                                                             \
        (((g_SysWork.playerWork.player.position.comp - Q12(base)) >= Q12(0.0f)) ? ((g_SysWork.playerWork.player.position.comp - Q12(base)) < Q12(tol)) : \
                                                                                  ((Q12(base) - g_SysWork.playerWork.player.position.comp) < Q12(tol)))

    #define MIN_OFFSET(x, neg, pos) \
        ((((x) + (-neg)) <= ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

    #define MAX_OFFSET(x, neg, pos) \
        ((((x) - (neg)) > ((x) + (pos))) ? ((x) - (neg)) : ((x) + (pos)))

    /** @brief Packs XZ cell coordinates in a single value.
     *
     * @param x X cell coordinate.
     * @param z Z cell coordinate.
     * @return Packed XZ cell coordinates.
     */
    #define CELL_XZ(x, z) \
        ((x) + ((z) << 8))

    #define HAS_FLAG(ptr, idx) \
        ((((u32*)ptr)[(idx) >> 5] >> ((idx) & 0x1F)) & (1 << 0))

    #define SET_FLAG(ptr, idx) \
        ((((u32*)ptr)[(idx) >> 5] |= (1 << 0) << ((idx) & 0x1F)))

    #define CLEAR_FLAG(ptr, idx) \
        ((((u32*)ptr)[(idx) >> 5] &= ~((1 << 0) << ((idx) & 0x1F))))

    /** @brief @deprecated Sync modes used by `DrawSync` and `VSync`. */
    enum e_SyncMode
    {
        SyncMode_Count     = -1,
        SyncMode_Wait      = 0,
        SyncMode_Immediate = 1,
        SyncMode_Wait2     = 2,
        SyncMode_Wait3     = 3,
        SyncMode_Wait8     = 8
    };

    /** @brief Paper map indices. Used for the navigation map screen. */
    enum e_PaperMapIdx
    {
        PaperMapIdx_OtherPlaces    = 0,
        PaperMapIdx_OldTown        = 1,
        PaperMapIdx_FogCentralTown = 2,
        PaperMapIdx_AltCentralTown = 3,
        PaperMapIdx_ResortTown     = 4,
        PaperMapIdx_FogSchoolBF    = 5,
        PaperMapIdx_FogSchool1F    = 6,
        PaperMapIdx_FogSchool2F    = 7,
        PaperMapIdx_FogSchoolRF    = 8,
        PaperMapIdx_AltSchoolBF    = 9,
        PaperMapIdx_AltSchool1F    = 10,
        PaperMapIdx_AltSchool2F    = 11,
        PaperMapIdx_AltSchoolRF    = 12,
        PaperMapIdx_FogSewer1F     = 13,
        PaperMapIdx_FogSewer2F     = 14,
        PaperMapIdx_AltSewer       = 15,
        PaperMapIdx_FogHospitalBF  = 16,
        PaperMapIdx_FogHospital1F  = 17,
        PaperMapIdx_FogHospital2F  = 18,
        PaperMapIdx_FogHospital3F  = 19,
        PaperMapIdx_AltHospitalBF  = 20,
        PaperMapIdx_AltHospital1F  = 21,
        PaperMapIdx_AltHospital2F  = 22,
        PaperMapIdx_AltHospital3F  = 23,
        PaperMapIdx_24             = 24
    };

    /** @brief System flags. TODO: Give this a more specific name. */
    enum e_SysFlags
    {
        SysFlag_None           = 0,
        SysFlag_0              = 1 << 0, /** @unused */
        SysFlag_DemoActive     = 1 << 1,
        SysFlag_2              = 1 << 2, /** @unused */
        SysFlag_CutsceneActive = 1 << 3,
        SysFlag_NoEnemySpawn   = 1 << 4,
        SysFlag_5              = 1 << 5, // Set during cafe cutscene. Something to prevent cutscene softlocks when an enemy is around??
        SysFlag_OnCameraRail   = 1 << 6,
        SysFlag_MenuActive     = 1 << 7,
        SysFlag_DoWarmReset    = 1 << 8,
        SysFlag_9              = 1 << 9, // @unused? Set during cafe cutscene along with `SysFlag_5` and never checked.
        SysFlag_LoadActive     = 1 << 10
    };

    /** @brief System process flags. */
    enum e_ProcessFlags
    {
        ProcessFlag_None              = 0,
        ProcessFlag_RoomTransition    = 1 << 0,
        ProcessFlag_OverlayTransition = 1 << 1,
        ProcessFlag_NewGame           = 1 << 2,
        ProcessFlag_LoadSave          = 1 << 3,
        ProcessFlag_Continue          = 1 << 4,
        ProcessFlag_BootDemo          = 1 << 5
    };

    /** @brief State IDs used by the main game loop. The values are used as indices into the `g_GameStateUpdateFuncs` function array. */
    enum e_GameState
    {
        GameState_Init,
        GameState_KonamiLogo,
        GameState_KcetLogo,
        GameState_AutoLoadSavegame,
        GameState_MovieIntroAlternate,
        GameState_MovieIntro,
        GameState_MainMenu,
        GameState_LoadSavegameScreen,
        GameState_MovieOpening,
        GameState_MainLoadScreen,
        GameState_InGame,
        GameState_MapEvent,
        GameState_ExitMovie,
        GameState_InventoryScreen,
        GameState_PaperMapScreen,
        GameState_SaveScreen,
        GameState_DebugMoviePlayer,
        GameState_OptionScreen,
        GameState_LoadStatusScreen,
        GameState_LoadMapScreen,
        GameState_Credits,
        GameState_Unk16 /** Removed debug menu? Doesn't exist in function array, but `DebugMoviePlayer` state tries to switch to it. */
    };

    /** @brief State IDs used by `GameState_InGame`.
     *
     * The values are used as indices into the 0x800A9A2C function array.
     */
    enum e_SysState
    {
        SysState_Gameplay       = 0,
        SysState_OptionsMenu    = 1,
        SysState_StatusMenu     = 2,
        SysState_MapScreen      = 3,
        SysState_Fmv            = 4,
        SysState_LoadOverlay    = 5,
        SysState_LoadRoom       = 6, // Alternative: SysState_LoadArea
        SysState_ReadMessage    = 7,
        SysState_SaveMenu0      = 8,
        SysState_SaveMenu1      = 9,
        SysState_EventCallback  = 10,
        SysState_EventSetFlag   = 11,
        SysState_EventPlaySound = 12,
        SysState_GameOver       = 13,
        SysState_GamePaused     = 14,
        SysState_Invalid        = 15 /** Used by `Event_Update` to signal that SysState shouldn't be updated. */
    };

    /** @brief Attack input types. Packed into a weapon attack using `WEAPON_ATTACK`. */
    enum e_AttackInputType
    {
        AttackInputType_Tap      = 0,
        AttackInputType_Hold     = 1,
        AttackInputType_Multitap = 2
    };

    /** @brief Game difficulties. */
    enum e_GameDifficulty
    {
        GameDifficulty_Easy   = -1,
        GameDifficulty_Normal = 0,
        GameDifficulty_Hard   = 1
    };

    /** @brief Game ending types. */
    typedef enum _GameEnding
    {
        GameEnding_0        = 0, // @unused? Credits has parameters set up for it, but nothing sets this.
        GameEnding_GoodPlus = 1,
        GameEnding_Good     = 2,
        GameEnding_BadPlus  = 3,
        GameEnding_Bad      = 4,
        GameEnding_Ufo      = 5
    } e_GameEnding;

    /** @brief Game ending flags. */
    typedef enum _GameEndingFlags
    {
        GameEndingFlag_GoodPlus = 1 << 0,
        GameEndingFlag_Good     = 1 << 1,
        GameEndingFlag_BadPlus  = 1 << 2,
        GameEndingFlag_Bad      = 1 << 3,
        GameEndingFlag_Ufo      = 1 << 4,

        // The following are only used in `currentEndingFlags`.
        GameEndingFlag_5        = 1 << 5, // Unknown purpose, `Ranking_PrepareSavegame` sets `g_SavegamePtr->currentEndingFlags |= 1 << 5`, nothing checks it?
        GameEndingFlag_6        = 1 << 6, // Set the first time ranking has been seen?
        GameEndingFlag_7        = 1 << 7  // Set if ranking has been seen and ending was UFO?
    } e_GameEndingFlags;

    /** @brief Game workspace. Stores miscellaneous gameplay-related data. */
    struct s_GameWork
    {
        s_OptionsConfig    config;
        s_ControllerData   controllers[CONTROLLER_COUNT_MAX];
        s_Savegame         autosave;
        s_Savegame         savegame;
        u16                gsScreenWidth;
        u16                gsScreenHeight;
        s_PrimColor        background2dColor;
        e_GameState        gameStatePrev;
        e_GameState        gameState;
        s32                gameStateSteps[3]; /** Sub-state steps used by the current `gameState`. Can be other state IDs or data. */
        s8                 unk_5A4[4];        // @unused?
        s32                field_5A8;
        s32                field_5AC;
        s8                 __pad_5B0;
        s8                 mapAnimIdx;
        s8                 bgmIdx;            /** `BgmTrackIdx` | Currently player background music track. */
        s8                 ambientIdx;        /** Index of `g_AmbientVabTaskLoadCmds`. */
        s_AnalogController rawController;
        s8                 unused_5BC[28];    // @unused Debug data?
    };

    /** @brief Extra player character info. */
    struct s_PlayerExtra
    {
        s_Model     model;             /** Manages upper half body's animations (torso, arms, head). */
        s32         disabledAnimBones; /** Bitfield of disabled animation bones. Can be created using the `BITMASK_RANGE` macro. */
        s32         state;             /** `e_PlayerState` */
        s32         upperBodyState;    /** `e_PlayerUpperBodyState` */
        s32         lowerBodyState;    /** `e_PlayerLowerBodyState` */
        e_InvItemId lastUsedItem;      /** Holds the last item ID used from inventory when the player is inside an item trigger area. */
    };

    /** @brief Player workspace.
     *
     * Possible original name: `shPlayerWork`.
     */
    struct s_PlayerWork
    {
        s_SubCharacter player;
        s_PlayerExtra  extra;
    };

    /** @brief Player combat info. */
    struct s_PlayerCombat
    {
        VECTOR3 attackPosition; /** Q19.12 */
        s8      __pad_C[3];
        s8      weaponAttack; /** Packed weapon attack. See `WEAPON_ATTACK`. */
        u8      currentWeaponAmmo;
        u8      totalWeaponAmmo;
        s8      weaponInventoryIdx; /** Index of the currently equipped weapon in the inventory. */
        u8      isAiming;           /** `bool` */
    };

    typedef union
    {
        s32 field_0; // Flags?
        struct
        {
            u8 field_0; // Flags. (1 << 0) Might be flashlight enabled.
            u8 field_1;
            u8 field_2;
            s8 __pad_3;
        } s_field_0;
    } u_Unk0;

    /** @brief Map effects info. */
    struct s_MapEffectsInfo
    {
        u_Unk0  field_0;
        q3_12   field_4; // Alpha.
        q3_12   field_6; // World tint color intensity.
        q3_12   worldTintR;
        q3_12   worldTintG;
        q3_12   worldTintB;
        u8      field_E; // Fog enabled if not set to 0, `Gfx_FogParametersSet` checks for values 0/1/2/3.
                         // Sets the transparent grey layer overlaid on characters and the enviroment.
        q19_12  fogDistance;
        CVECTOR fogColor;
        u8      enableTintLightOverlap; /** `bool` */
        CVECTOR pointLightTint;         /** Volumetric point light color. */
        CVECTOR worldTint;              /** Subtractive. */
        CVECTOR field_21;               // Particle effect related. Only the first value affects snow transparency.
        CVECTOR field_25;
    };

    typedef struct
    {
        s_MapEffectsInfo effectsInfo;
        q3_12            flashlightLensFlareIntensity;
        q3_12            brightnessIntensity;
        q19_12           fogDistance;
    } s_StructUnk3;
    STATIC_ASSERT_SIZEOF(s_StructUnk3, 52);

    // Extra effects info?
    typedef struct
    {
        s32             primitiveType; /** `e_PrimitiveType` */
        s8*             field_4;       /** Points to different types of data depending on `field_0`. */
        s32             field_8;       // } Q19.12?
        s32             field_C;       // }
        s32             field_10;
        u8              field_14;                /** `bool` */
        u8              isFlashlightOn;          /** `bool` */
        u8              isFlashlightUnavailable; /** `bool` */
        q3_12           flashlightIntensity;
        u16             field_1A;
        s_StructUnk3    field_1C[2];
        s_StructUnk3    field_84[2];
        s_StructUnk3    field_EC[2];
        s_StructUnk3    field_154;
    } s_SysWork_2388;

    /** @brief Main system workspace. Stores key engine data. */
    struct s_SysWork
    {
        int  sysState;             /** `e_SysState` */
        int  sysStateSteps[3];     /** Temp data used by current `sysState`. Can be another state ID or other data. */
        int  sysStateStepData[2];  /** Temporary data for `sysStateSteps[0]` and `sysStateSteps[1]`. */
        int  sysStateCounter;      /** Frame counter for the current `sysState`. Reset when `sysState` is changed. */
        int  gameStateCounter;     /** Frame counter for the current `g_GameWork.gameState`. Reset when `gameState` is changed. */
        int  gameStateStepCounter; /** Frame counter for the current `g_GameWork.gameStateSteps[0]`. Reset when `gameStateSteps[0]` is changed. */
        int  cutsceneBorderState;  /** `e_CutsceneBorderState` */
        bool isMgsStringSet;       /** Indicates if string have been loaded and is going (or it is) being display. */

        s_PlayerCombat playerCombat;
        s_PlayerWork   playerWork;
        s_SubCharacter npcs[NPC_COUNT_MAX];
        GsCOORDINATE2  playerBoneCoords[HarryBone_Count];
        GsCOORDINATE2  unkCoords_E30[5];                       // Might be part of previous array for 5 extra coords which go unused.
        GsCOORDINATE2  npcBoneCoordBuffer[NPC_BONE_COUNT_MAX]; /** Contiguous NPC bone coord buffer. */
        s8             npcFlagsId;                             // 1-based NPC ID for `npcFlags`.

        s8               loadingScreenIdx;
        s8               areaTransitionFlags;                /** `e_AreaTransitionFlags` */
        s8               sfxPairIdx;                         /** `e_SfxPairIdx` | Index into `SFX_PAIRS`. */
        u16              charaGroupFlags[CHARA_GROUP_COUNT]; /** `e_CharaGroupFlags` */
                                                             // Enabling a flag for Larval Stalkers causes them to die.
        s32              field_228C[1];
        s32              npcFlags;         // Flags related to NPCs. Each bit corresponds to an `npcs` array entry.
        s32              processFlags;     /** `e_ProcessFlags` */
        s32              unused_229C;      /** @unused Set to `NO_VALUE` when the player has been initalized and 0 when the player changes areas. Beyond that it's dead code. */
        s32              bgmStatusFlags;   /** `e_BgmStatusFlags` */
        s32              sysFlags;         // `e_SysFlags` | `SysFlag_OnCameraRail` passed as "use through door cam" flag in `vcSetFirstCamWork`.
        GsCOORDINATE2    coord_22A8;       // For particles only?
        GsCOORDINATE2    coord_22F8;       // Likely related to above.
        s8               field_2348   : 8; // Related to particles.
        s8               field_2349   : 8; // Particle spawn multiplier?
        u8               field_234A   : 8; /** `bool` */
        u8               field_234B_0 : 4; /** `bool` | Related to particles. Used to trigger SFX? */
        u8               field_234B_4 : 4; // Related to particles.
        q19_12           mapMsgTimer;
        bool             enableHalfHeightGlyphs;
        u8               silentYesSelection : 4; /** `bool` */
        u32              invItemSelectedIdx : 8;
        u32              invItemLoadFlags   : 8; /** `e_InvItemLoadFlags` */
        s8               targetNpcIdx;           /** Index of the NPC in `npcs` being targeted by the player. */
        s8               npcIdxs[CHARA_GROUP_COUNT];
        u8               enablePlayerMatchAnim; /** `bool` | Activates the animation performed by Harry when lighting a match at the beginning of the game. */
        u8               playerStopFlags;       /** `e_PlayerStopFlags` */
        GsCOORDINATE2*   lightBoneCoord;
        VECTOR3          lightPosition;      // } Often set to DMS cutscene data.
        GsCOORDINATE2*   lensFlareBoneCoord; // }
        SVECTOR          lightRotation;      // }
        q3_12            lightIntensity;
        q3_12            cameraAngleY;
        q3_12            cameraAngleZ;
        s16              field_237E;
        q19_12           cameraRadiusXz;
        q19_12           cameraY;
        s_SysWork_2388   field_2388;
        s32              field_2510;                       // } Related to libkpad.
        s_SysWork_2514   field_2514;                       // }
        s8               field_254C[508];                  /** Used through indirect pointer calls. Tied to `libkpad`.*/
        q3_12            bgmLayerVolumes[BGM_LAYER_COUNT]; // Last index value is not a layer, but some sort of timer. See `Bgm_LayersUpdate`.
        q23_8            field_275C; // } SFX volumes?
        q23_8            field_2760; // }
        q23_8            field_2764; // }
    };

    extern void* g_OvlBodyprog;
    extern void* g_OvlDynamic;

    extern s_SysWork         g_SysWork;
    extern s_GameWork        g_GameWork;
    extern s_GameWork* const g_GameWorkConst;
    extern s_GameWork* const g_GameWorkPtr;

    /** @brief Sets `sysState` in `g_SysWork` for the next tick.
     *
     * @param sysState System state to set.
     * @return New system state.
     */
    static inline e_SysState SysWork_StateSetNext(e_SysState sysState)
    {
        auto state                    = sysState;
        g_SysWork.sysState            = sysState;
        g_SysWork.sysStateCounter     = 0;
        g_SysWork.sysStateSteps[0]    = 0;
        g_SysWork.sysStateStepData[0] = 0;
        g_SysWork.sysStateSteps[1]    = 0;
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
        return state;
    }

    /** @brief Increments `sysStateStep` in `g_SysWork` for the next tick.
     *
     * @param stepIdx Index of the `sysStateStep` to increment.
     */
    static inline void SysWork_StateStepIncrement(int stepIdx)
    {
        if (stepIdx == 0)
        {
            g_SysWork.sysStateStepData[0] = 0;
            g_SysWork.sysStateSteps[1]    = 0;
            g_SysWork.sysStateStepData[1] = 0;
            g_SysWork.sysStateSteps[2]    = 0;
            g_SysWork.sysStateSteps[0]++;
        }
        else if (stepIdx == 1)
        {
            g_SysWork.sysStateStepData[1] = 0;
            g_SysWork.sysStateSteps[2]    = 0;
            g_SysWork.sysStateSteps[1]++;
        }
        else
        {
            g_SysWork.sysStateSteps[2]++;
        }
    }

    /** @brief Sets `sysStateStep[stepIdx]` in `g_SysWork` for the next tick.
     *
     * @param stepIdx The `sysStateStep` index to change.
     * @param sysStateStep System state step to set.
     * @return New system state step.
     */
    static inline int SysWork_StateStepSet(int stepIdx, int sysStateStep)
    {
        int step = 0;
        if (stepIdx == 0)
        {
            step                          = 
            g_SysWork.sysStateSteps[0]    = sysStateStep;
            g_SysWork.sysStateStepData[0] = 0;
            g_SysWork.sysStateSteps[1]    = 0;
            g_SysWork.sysStateStepData[1] = 0;
            g_SysWork.sysStateSteps[2]    = 0;
        }
        else if (stepIdx == 1)
        {
            step                          = 
            g_SysWork.sysStateSteps[1]    = sysStateStep;
            g_SysWork.sysStateStepData[1] = 0;
            g_SysWork.sysStateSteps[2]    = 0;
        }
        else
        {
            step                       =
            g_SysWork.sysStateSteps[2] = sysStateStep;
        }

        return step;
    }

    /** @brief Resets `sysStateStep` in `g_SysWork` for the next tick. */
    static inline void SysWork_StateStepReset()
    {
        g_SysWork.sysStateSteps[0]    = NO_VALUE;
        g_SysWork.sysStateStepData[0] = 0;
        g_SysWork.sysStateSteps[1]    = 0;
        g_SysWork.sysStateStepData[1] = 0;
        g_SysWork.sysStateSteps[2]    = 0;
    }

    /** @brief Sets an NPC flag in the `g_SysWork.npcFlags` bitfield.
     *
     * @param flagIdx Index of the NPC flag to set.
     */
    static inline void SysWork_NpcFlagSet(int flagIdx)
    {
        g_SysWork.npcFlags |= 1 << flagIdx;
    }

    /** @brief Clears an NPC flag in the `g_SysWork.npcFlags` bitfield.
     *
     * @param flagIdx Index of the NPC flag to clear.
     */
    static inline void SysWork_NpcFlagClear(int flagIdx)
    {
        CLEAR_FLAG(&g_SysWork.npcFlags, flagIdx);
    }

    /** @brief Sets the game state to be used for the next tick all resets all state steps.
     *
     * Records the outgoing state as `gameStatePrev`, sets `gameState` as the new 
     * state, and clears all state steps for the new state to have a clean slate.
     *
     * `gameStateCounter` and `gameStateStepCounter` are also cleared and the sys state is changed to `SysState_Gameplay`.
     *
     * @note Changed from inline to macro to fix some stubborn functions.
     *
     * @param gameState New game state to enter.
     */
    static inline void Game_StateSetNext_ClearStateSteps(e_GameState gameState)
    {
        auto prevState = g_GameWork.gameState;

        g_GameWork.gameState           = gameState;
        g_SysWork.gameStateCounter     = 0;
        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;

        SysWork_StateSetNext(SysState_Gameplay);

        g_GameWork.gameStateSteps[1] = 0;
        g_GameWork.gameStateSteps[2] = 0;
        g_GameWork.gameStateSteps[0] = prevState;
        g_GameWork.gameStatePrev     = prevState;
        g_GameWork.gameStateSteps[0] = 0;
    }

    /** @brief Sets the GameState to be used in the next game update.
     * Inlined into `stream` and `b_konami`.
     */
    static inline void Game_StateSetNext(e_GameState gameState)
    {
        auto prevState = g_GameWork.gameState;

        g_GameWork.gameState           = gameState;
        g_SysWork.gameStateCounter     = 0;
        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;

        SysWork_StateSetNext(SysState_Gameplay);

        g_GameWork.gameStateSteps[0] = prevState;
        g_GameWork.gameStatePrev     = prevState;
        g_GameWork.gameStateSteps[0] = 0;
    }

    /** @brief Returns the GameState to the previously used state and resets all state steps.
     *
     * Records the outgoing state as `gameStatePrev`, sets `gameState` to the previous 
     * `gameStatePrev` value, and clears all state-steps for the new state to have a clean slate.
     *
     * @note `gameStateCounter` and `gameStateStepCounter` are also cleared, and SysState is changed to `SysState_Gameplay`.
     */
    static inline void Game_StateSetPrevious()
    {
        auto prevState = g_GameWork.gameState;

        g_SysWork.gameStateCounter     = 0;
        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;

        SysWork_StateSetNext(SysState_Gameplay);

        g_GameWork.gameStateSteps[0] = prevState;
        g_GameWork.gameState         = g_GameWork.gameStatePrev;
        g_GameWork.gameStatePrev     = prevState;
        g_GameWork.gameStateSteps[0] = 0;
    }

    /** @brief Sets one of the three game state step counters.
     *
     * The steps form a hierarchy used by the game's game state state machines:
     * - [0] = state step
     * - [1] = sub-step,
     * - [2] = sub-sub-step.
     *
     * @note Writing a step cascades a reset downward: changing a higher
     * level invalidates the steps nested beneath it, so all levels lower
     * than `stepIdx` are reset to 0.
     * Setting [0] additionally clears the `gameStateStepCounter` frame counter.
     *
     * @param stepIdx Step index to set (0, 1, or 2).
     * @param stateStep New value for the index.
     * @return Value written (`== stateStep`).
     */
    static inline int Game_StateStepSet(int stepIdx, int stateStep)
    {
        int step = 0;
        if (stepIdx == 0)
        {
            step                           = 
            g_GameWork.gameStateSteps[0]   = stateStep;
            g_SysWork.gameStateStepCounter = 0;
            g_GameWork.gameStateSteps[1]   = 0;
            g_GameWork.gameStateSteps[2]   = 0;
        }
        else if (stepIdx == 1)
        {
            step                         =
            g_GameWork.gameStateSteps[1] = stateStep;
            g_GameWork.gameStateSteps[2] = 0;
        }
        else
        {
            step                         =
            g_GameWork.gameStateSteps[2] = stateStep;
        }

        return step;
    }

    static inline void Game_StateStepIncrement(int stepIdx)
    {    
        if(stepIdx == 0)
        {
            g_SysWork.gameStateStepCounter = 0;
            g_GameWork.gameStateSteps[1]   = 0;
            g_GameWork.gameStateSteps[2]   = 0;
            g_GameWork.gameStateSteps[0]++;
        }
        else if(stepIdx == 1)
        {
            g_GameWork.gameStateSteps[1]++;
            g_GameWork.gameStateSteps[2] = 0;
        }
        else
        {
            g_GameWork.gameStateSteps[2]++;
        }
    }
}

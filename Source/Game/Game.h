#pragma once

#include "Psx.h"
#include "Game/libkpad.h"

#include "Game/Bodyprog/Chara/Anim.h"
#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Events/Bgm.h"
#include "Game/Bodyprog/Formats/Anm.h"
#include "Game/Bodyprog/Items.h"
#include "Game/Bodyprog/Chara/Model.h"
#include "Game/Bodyprog/Savegame.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Maps/Characters/Harry.h"

namespace Silent::Game
{
    /* Macros for comparing map chunk indices against a map X/Z position.
    *
    * SH1 game world is split to 40x40 meter chunks, terrain loading code and filenames of IPDs (eg. `THR01FF.IPD` for X 1, Z -1)
    * use a rounded-down grid where chunk 0 is anything between 0 to 40, chunk -1 is anything between -40 to 0.
    *
    * The code used in map event/object spawning code number them differently, since these divide positions with
    * C's truncating division which would give 0 for anything between -40 to 40 (making chunk "0" count as 80 units).
    * To workaround that the code adds 1 if position is positive, or subtracts 1 for negative (or 0).
    * This workaround prevents the actual compiled code from seeing any position as chunk 0 (while other parts of game code do
    * make use of chunk 0), and makes code for any checks for chunk 0 or above +1 of the actual index being checked.
    *
    * To make our code that uses these macros consistent with rest of game code, we make our macro adjust the
    * index being checked for, so the source code using the macros can pass in rounded-down chunk indices, and that'll
    * handle adding 1 to any index that's positive/zero.
    * This lets it match the original compiled ASM for positive chunk checks (that showed it checking +1 of the actual chunk index),
    * but lets our C code make use of the same indices used in the rest of the game.
    */

    /** @brief Map event chunk index of world coordinate `pos`, never returns chunk 0, see above. */
    #define MAP_CHUNK_IDX(pos) \
        (((pos) / Q12(CHUNK_SIZE)) + (((pos) > Q12(0.0f)) ? 1 : -1))

    /** @brief Converts rounded-down terrain chunk grid index `n` to its equivalent `MAP_CHUNK_IDX` value. */
    #define MAP_CHUNK_IDX_FROM_GRID(n) \
        (((n) >= 0) ? ((n) + 1) : (n))

    #define PLAYER_IN_MAP_CHUNK_X(x) \
        (MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vx) == MAP_CHUNK_IDX_FROM_GRID(x))

    #define PLAYER_IN_MAP_CHUNK_Z(z) \
        (MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vz) == MAP_CHUNK_IDX_FROM_GRID(z))

    /** @brief Checks if the player is in terrain chunk (`x`, `z`). */
    #define PLAYER_IN_MAP_CHUNK(x, z) \
        (PLAYER_IN_MAP_CHUNK_X(x) && PLAYER_IN_MAP_CHUNK_Z(z))

    /** @brief `CELL_XZ` switch key of terrain chunk (`x`, `z`). Indices are offset by 16 to keep both positive? (or maybe `SUBCHUNK_SIZE * 2`?) */
    #define MAP_CHUNK_CELL(x, z) \
        CELL_XZ(MAP_CHUNK_IDX_FROM_GRID(x) + 16, MAP_CHUNK_IDX_FROM_GRID(z) + 16)

    /** @brief `MAP_CHUNK_CELL` switch key of the player's current chunk. */
    #define PLAYER_MAP_CHUNK_CELL()                                          \
        CELL_XZ(MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vx) + 16, \
                MAP_CHUNK_IDX(g_SysWork.playerWork.player.position.vz) + 16)

    #define PLAYER_NEAR_POS(comp, base, tol)                                                                                                             \
        (((g_SysWork.playerWork.player.position.comp - Q12(base)) >= Q12(0.0f)) ? ((g_SysWork.playerWork.player.position.comp - Q12(base)) < Q12(tol)) : \
                                                                                  ((Q12(base) - g_SysWork.playerWork.player.position.comp) < Q12(tol)))

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
        GameState_LanguageScreen,
        GameState_SplashScreen,
        GameState_LogosScreen,
        GameState_MovieIntro,
        GameState_AutoLoadSavegame,
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
        GameState_OptionScreen,
        GameState_LoadStatusScreen,
        GameState_LoadMapScreen,
        GameState_Credits
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
        int                gameStateSteps[3]; /** Sub-state steps used by the current `gameState`. Can be other state IDs or data. */
        s32                field_5A8;
        s32                field_5AC;
        int                mapAnimIdx;
        int                bgmIdx;            /** `BgmTrackIdx` | Currently player background music track. */
        int                ambientIdx;        /** Index of `g_AmbientVabTaskLoadCmds`. */
        s_AnalogController rawController;
    };

    /** @brief Extra player character info. */
    struct s_PlayerExtra
    {
        s_Model     model;                 /** Manages upper half body's animations (torso, arms, head). */
        int         disabledAnimBoneFlags; /** Created using `BITMASK_RANGE`. */
        int         state;                 /** `e_PlayerState` */
        int         upperBodyState;        /** `e_PlayerUpperBodyState` */
        int         lowerBodyState;        /** `e_PlayerLowerBodyState` */
        e_InvItemId lastUsedItem;          /** Holds the last item ID used from inventory when the player is inside an item trigger area. */
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
        VECTOR3 attackPosition;     /** Q19.12 */
        int     weaponAttack;       /** Packed weapon attack. See `WEAPON_ATTACK`. */
        int     currentWeaponAmmo;
        int     totalWeaponAmmo;
        int     weaponInventoryIdx; /** Index of the currently equipped weapon in the inventory. */
        bool    isAiming;
    };

    /** @brief World environment preset. Contains fog, lighting, and tint settings. */
    struct s_WorldEnvPreset
    {
        u8      flags;                  /** `e_WorldEnvFlags` */
        u8      lightLevelMax;          /** Upper limit on the computed light level of lit vertices. `0xFF` for no limit. */
        u8      lightMode;              /** `e_WorldEnvLightMode` */
        q3_12   spotLightIntensity;
        q3_12   worldLightIntensity;
        q3_12   worldTintR;
        q3_12   worldTintG;
        q3_12   worldTintB;
        u8      fogMode;                /** `e_WorldEnvFogMode` */
        q19_12  fogDistance;
        CVECTOR fogColor;
        u8      enableTintLightOverlap; /** `bool` */
        CVECTOR pointLightTint;         /** Volumetric point light color. */
        CVECTOR worldTint;              /** Subtractive. */
        CVECTOR snowColor;              /** Snow particle color. `Particle_SnowDraw` uses only the red value in maps with header `field_16 == 1`. } Both multiply a dim additive texture, colors in the default env presets likely make very little difference? */
        CVECTOR rainColor;              /** Rain particle color. `Particle_RainDraw` uses only the red value in maps with header `field_16 == 1`. } */
    };

    /** @brief World environment state. */
    struct s_WorldEnvState
    {
        s_WorldEnvPreset effectsInfo;
        q3_12            flashlightLensFlareIntensity;
        q3_12            brightnessIntensity;
        q19_12           fogDistance;
    };

    /** @brief Game-side world environment, lighting and fog presets in use, and flashlight state.
     *
     * Each frame `WorldEnv_Update` works out `current` in two steps:
     * 1. `transition` fades from the previous presets (`transitionFrom`) toward the new ones (`transitionTo`).
     * 2. `current` is a mix of the flashlight off and on entries of `transition`, weighted by `flashlightIntensity`.
     *
     * `WorldEnv_Apply` then copies `current` into `g_WorldEnvRenderWork` for drawing.
     *
     * The `transition***` arrays are indexed by the flashlight state: 0 off, 1 on.
     */
    struct s_WorldEnvWork
    {
        s32             progressType;            /** `e_PrimitiveType` of `progressValue`. `PrimitiveType_None`: no transition. */
        s8*             progressValue;           /** Value that drives the transition. Usually overlay data. */
        q19_12          progressStart;           /** `progressValue` at which the transition starts. */
        q19_12          progressEnd;             /** `progressValue` at which the transition completes. */
        q19_12          visibilityDistance;      /** @unused Result of `func_8003FEC0`, never read. */
        u8              flashEffect;             /** `bool` | Trigger world flash effect (used for weapons). */
        u8              isFlashlightOn;          /** `bool` */
        u8              isFlashlightUnavailable; /** `bool` | Set when both presets are the same. Forces the flashlight off. */
        q3_12           flashlightIntensity;
        s_WorldEnvState transition[2];        /** Current blend of `transitionFrom` and `transitionTo`. */
        s_WorldEnvState transitionTo[2];      /** Most recently selected presets. */
        s_WorldEnvState transitionFrom[2];    /** Copy of `transition` taken when a transition starts. */
        s_WorldEnvState current;              /** Final blended state. Read by gameplay code for the environment flags. */
    };

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
        bool isMsgStringSet;       /** Indicates if string have been loaded and is going (or it is) being display. */

        s_PlayerCombat playerCombat;
        s_PlayerWork   playerWork;
        s_SubCharacter npcs[NPC_COUNT_MAX];

        // @deprecated Using `s_SubCharacter::model.anim.BoneTransforms` in new code.
        GsCOORDINATE2 playerBoneCoords[HarryBone_Count];
        GsCOORDINATE2 npcBoneCoordBuffer[NPC_BONE_COUNT_MAX]; /** Contiguous NPC bone coord buffer. */

        int npcFlagId; /** 1-based NPC ID for `npcFlags`. */

        int loadingScreenIdx;
        int areaTransitionFlags;                /** `e_AreaTransitionFlags` */
        int sfxPairIdx;                         /** `e_SfxPairIdx` | Index into `SFX_PAIRS`. */
        int charaGroupFlags[CHARA_GROUP_COUNT]; /** `e_CharaGroupFlags` */ // Enabling a flag for Larval Stalkers causes them to die.
        s32 field_228C[1];
        int npcFlags;                           // Flags related to NPCs. Each bit corresponds to an `npcs` array entry.
        int processFlags;                       /** `e_ProcessFlags` */
        int bgmStatusFlags;                     /** `e_BgmStatusFlags` */
        int sysFlags;                           // `e_SysFlags` | `SysFlag_OnCameraRail` passed as "use through door cam" flag in `vcSetFirstCamWork`.

        GsCOORDINATE2 coord_22A8;   // For particles only?
        GsCOORDINATE2 coord_22F8;   // Likely related to above.
        s8            field_2348;   // Related to particles.
        s8            field_2349;   // Particle spawn multiplier?
        bool          field_234A;
        bool          field_234B_0; /** Related to particles. Used to trigger SFX? */
        u8            field_234B_4; // Related to particles.

        q19_12 mapMsgTimer;
        bool   enableHalfHeightGlyphs;
        bool   silentYesSelection;
        int    invItemSelectedIdx;
        int    invItemLoadFlags; /** `e_InvItemLoadFlags` */

        int  targetNpcIdx;     /** Index of the NPC in `npcs` being targeted by the player. */
        int  npcIdxs[CHARA_GROUP_COUNT];
        bool enablePlayerMatchAnim; /** `bool` | Activates the animation performed by Harry when lighting a match at the beginning of the game. */
        int  playerStopFlags;       /** `e_PlayerStopFlags` */

        GsCOORDINATE2* lightBoneCoord;
        VECTOR3        lightPosition;      // } Often set to DMS cutscene data.
        GsCOORDINATE2* lensFlareBoneCoord; // }
        SVECTOR        lightRotation;      // }
        q3_12          lightIntensity;

        q3_12  cameraAngleY;
        q3_12  cameraAngleZ;
        q19_12 cameraRadiusXz;
        q19_12 cameraY;

        s_WorldEnvWork worldEnv;

        //s_KPad_Handler* kpadHandlers; // Head of the kpad handler list from `KPad_HandlerList_Init`.
        //s_KPadWork      kpad;
        //s_KPad_Channel  kpadChannels[2];
        //s_KPad_Effect   kpadEffects[16];

        q3_12 bgmLayerVolumes[BGM_LAYER_COUNT]; // Last index value is not a layer, but some sort of timer. See `Bgm_LayersUpdate`.
        q23_8 field_275C;                       // } SFX volumes?
        q23_8 field_2760;                       // }
        q23_8 field_2764;                       // }
    };

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

    /** @brief Increments one of the three game state step counters.
     *
     * The steps form a hierarchy used by the `gameState` machines:
     *   [0] = state step, [1] = sub-step, [2] = sub-sub-step.
     *
     * @note Incrementing a step cascades a reset downward: changing a higher 
     * level invalidates the steps nested within it, so all levels lower
     * than `stepIdx` are reset to 0.
     * Incrementing [0] additionally clears the `gameStateStepCounter` frame counter.
     *
     * @param stepIdx Step index to increment: 0, 1, or 2.
     */
    static inline void Game_StateStepIncrement(int stepIdx)
    {    
        if (stepIdx == 0)
        {
            g_SysWork.gameStateStepCounter = 0;
            g_GameWork.gameStateSteps[1]   = 0;
            g_GameWork.gameStateSteps[2]   = 0;
            g_GameWork.gameStateSteps[0]++;
        }
        else if (stepIdx == 1)
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

#pragma once

#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/EventFlags.h"
#include "Game/Bodyprog/Items.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Utils/Bitfield.h"

using namespace Silent::Utils;

namespace Silent::Game
{
    /** @brief Savegame data. */
    struct s_Savegame
    {
        s_InventoryItem items[INV_ITEM_COUNT_MAX];
        s8              field_A0;
        int             mapIdx;       /** `e_MapIdx` Index to overlay `.BIN` files. */
        int             mapRoomIdx;   /** Index to local map geometry IPD files. */
        int             savegameCount;
        int             locationId;                  /** `e_SaveLocationId` */
        int             paperMapIdx;                 /** `e_PaperMapIdx` | Index of the paper map displayed when opening the map screen. */
        int             equippedWeapon;              /** `e_InvItemId` | Affects the visible player weapon model. */
        int             inventorySlotCount;          /** Item slots. */
        u32             itemToggleFlags;             /** `e_ItemToggleFlags` */
        s32             mapEnemyStates[Chara_Count]; /** Flags indicating the enemy states in a given overlay.
                                                     * All set to 1 by default. As soon as they are killed (not just stunned),
                                                     * set to 0 based on a currently unknown index value.
                                                     */
        Bitfield        paperMapFlags;               // See Sparagas' `HasMapsFlags` struct for details of every bit.
        Bitfield        eventFlags = Bitfield(EventFlag_Count); /** Accessed via `Savegame_EventFlagGet` and `Savegame_EventFlagSet`. */
                                                     // grouped by location and not item types.
                                                     // Also includes map marking flags - see Sparagas' `MapMarkingsFlags` struct for details of every bit.
        q19_12          healthSaturation;            /** Range: [0, 300]. Ampoules give extra stored health. If the player loses health, it will be slowly restored. */
        int             pickedUpItemCount;
        u8              inventoryItemFlags; /** `e_InvItemFlags` */
        q19_12          playerHealth;       /** Default: `Q12(100.0f)` */
        q19_12          playerPositionX;
        q3_12           playerRotationY;  /** Range [0, 0.999755859375], positive Z: 0, clockwise rotation. It can be multiplied by 360 to get degrees. */
        int             clearGameCount;   /** Range [0, 99] */
        u8              clearGameEndings; /** `e_GameEndingFlags` */
        q19_12          playerPositionZ;
        q20_12          gameplayTimer;
        q20_12          runDistance;
        q20_12          walkDistance;
        u8              isNextFearMode           : 1; /** `bool` | Makes savegame entry text gold. */
        u8              add290Hours              : 2; /** Adds 290 hours per 1 bit, i.e. 290, 580, 870. */
        u8              pickedUpSpecialItemCount : 5; /** Red/None: 0?, Yellow: 8, Green: 16, @unused Rainbow: 24. */
                                                     /** Sparagas' investigations indicate this variable should be
                                                      * two different variables. However, splitting it causes minor
                                                      * mismatches in some functions.
                                                      *
                                                      * The first 3 bits indicate the number of special items the
                                                      * player has picked up, and the last 2 bits indicate the color of the Hyper
                                                      * Blaster beam.
                                                      *
                                                      * Belek666 suggests that some functions specifically access this field as 5 bits.
                                                      *
                                                      * The the odd access results in a bug where the results screen will count more collected
                                                      * special items than normal by additionally reading one of the two bits
                                                      * for the Hyper Blaster beam color.
                                                      */
        int             meleeKillCount;
        int             meleeKillCountB; // Can't be packed if used as `u16`.
        int             rangedKillCount;
        u32             field_260      : 28;
        s32             gameDifficulty : 4;  /** `e_GameDifficulty` */
        int             firedShotCount;      /** Missed shot count = firedShotCount - (closeRangeShotCount + midRangeShotCount + longRangeShotCount). */
        int             closeRangeShotCount; /** Only hits counted. */
        int             midRangeShotCount;   /** Only hits counted. */
        int             longRangeShotCount;  /** Only hits counted. */
        u16             field_26C;
        u16             field_26E; // Related to enemy kills.
        u16             field_270;
        u16             field_272;
        u16             field_274;
        u16             field_276;
        u16             field_278;
        s8              field_27A; /** `e_GameEndingFlags` | Ending reached in the current session.
                                    * Similar to `clearGameEndings`, but the value is replaced with the current ending instead of ORed.
                                    * Used to identify the latest ending for the ranking screen.
                                    */
        int             continueCount;
    };

    /** @brief User options configuration. */
    struct s_OptionsConfig
    {
        s_ControllerConfig controllerConfig;
        s8                 screenPositionX;     /** Range: [-11, 11], default: 0. */
        s8                 screenPositionY;     /** Range: [-8, 8], default: 0. */
        u8                 soundType;           /** `bool` | Stereo: `false`, Monaural: `true`, default: Stereo. */
        u8                 volumeBgm;           /** Range: [0, 128] with steps of 8, default: 16. */
        u8                 volumeSe;            /** Range: [0, 128] with steps of 8, default: 16. */
        u8                 vibrationEnabled;    /** `bool` | Off: 0, On: 128, default: On. */
        u8                 brightness;          /** Range: [0, 7], default: 3. */
        u8                 extraWeaponCtrl;     /** `bool` | Switch: `false`, Press: `true`, default: Press. */
        u8                 extraBloodColor;     /** `e_BloodColor` | Default: Normal. */
        s8                 autoLoad;            /** `bool` | Off: `false`, On: `true`, default: Off. */
        s8                 unused_26;           /** @unused */
        u8                 extraOptionsEnabled; /** Holds unlocked option flags. */
        s8                 extraViewCtrl;       /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
        s8                 extraViewMode;       /** `bool` | Normal: `false`, Self View: `true`, default: Normal. */
        s8                 extraRetreatTurn;    /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
        s8                 extraWalkRunCtrl;    /** `bool` | Normal: `false`, Reverse: `true`, default: Normal. */
        s8                 extraAutoAiming;     /** `bool` | On: `false`, Off: `true`, default: On. */
        s8                 extraBulletAdjust;   /** x1-x6: Range [0, 5], default: x1. */
        Bitfield           seenGameOverTips;    /** Bitfield tracking seen game-over tips. Each bit corresponds to a tip index (0–14), set bits indicate seen tips. Resets after picking all 15. */
        s8                 unk_30[4];
        u32                palLanguageId;
    };

    extern s_Savegame* const g_SavegamePtr;

    /** @brief Gets an event flag state from the savegame event flags array.
     *
     * @param flagIdx Event flag index.
     * @return Event flag state.
     */
    static inline bool Savegame_EventFlagGet(int flagIdx)
    {
        return g_SavegamePtr->eventFlags.Test(flagIdx);
    }
    #define Savegame_EventFlagGetAlt Savegame_EventFlagGet

    /** @brief Clears an event flag state in the savegame event flags array.
     *
     * @param flagIdx Event flag index.
     */
    static inline void Savegame_EventFlagClear(int flagIdx)
    {
        g_SavegamePtr->eventFlags.Clear(flagIdx);
    }

    /** @brief Sets an event flag state in the savegame event flags array.
     *
     * @param flagIdx Event flag index.
     */
    static inline void Savegame_EventFlagSet(int flagIdx)
    {
        g_SavegamePtr->eventFlags.Set(flagIdx);
    }
    #define Savegame_EventFlagSetAlt Savegame_EventFlagSet
}

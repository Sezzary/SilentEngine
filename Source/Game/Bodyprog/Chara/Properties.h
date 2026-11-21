#pragma once

#include "Game/Bodyprog/Chara/Combat.h"

namespace Silent::Game
{
    // TODO: Unsure if this struct is puppet doctor specific or shared with all characterss. Pointer gets set at puppetDoc+0x124.
    typedef struct
    {
        q19_12      health_0;
        s32         field_4;
        s32         field_8;
        s32         field_C;
        s8          unk_10[8];
        q19_12      field_18; // Angle?
        s32         idx_1C;
        s32         field_20;
        s_AnimInfo* animInfo_24;
        s8          unk_28[4];
        q19_12      field_2C;
        s8          unk_30[4];
    } s_800D5710;

    typedef union
    {
        s32 val32;
        s16 val16[2];
        s8  val8[4];
    } u_Property;

    /** @brief Temporary struct. */
    typedef struct _PropsDummy
    {
        u_Property properties_E8[16];
    } s_PropsDummy;

    /** @brief Player character properties. */
    typedef struct _PropsPlayer
    {
        q19_12 afkTimer; // Increments every tick for 10 seconds before AFK anim starts.
        q19_12 floorHeight;
        q19_12 field_F0;
        q19_12 field_F4; // Angle related to X axis flex rotation.
        s32    runStepSfxCount;
        q19_12 exhaustionTimer;
        q19_12 field_100;    // Angle?
        q19_12 field_104;    // Distance?
        q19_12 runDistance;
        u8     field_10C;    // Player SFX pitch?
        u8     field_10D;    // @unused? Set to 0 or 1.
        s8     __pad_10E[2];
        q19_12 timer_110; // Increases when `flags & CharaFlag_Unk4` is set, reset when reaches `D_800C45EC`.
        q19_12 gasWeaponPowerTimer; /** Timer for the Chainsaw and Rock Drill power. */
        s16    field_118; // q3_12?
        s8     __pad_11A[2];
        s32    flags; /** `e_PlayerFlags` */
        q3_12  quickTurnHeadingAngle; /** Target quick turn heading angle. */
        q3_12  field_122; // Specially used when aiming an enemy. Y angle delta to target? 
        q3_12  headingAngle;
        q3_12  moveSpeed; // Used to indicate how much the player should move foward. Seems to be squared.
    } s_PropsPlayer;

    // Used by Alessa, Bloody Incubator, BloodyLisa, Cheryl, Cybil, Dahlia, Ghost Child Alessa, Ghost Doctor, Incubator,
    // Kaufmann, Lisa.
    typedef struct _PropsNpc
    {
        s32    controlState; /** `e_*Control` */
        s16    field_EC;
        s16    field_EE;
        s32    freeze; // `bool`
        s32    field_F4;
        s32    resetControlState; // `bool`
        s8     __pad_FC[2];
        s16    field_FE; // Index.
        q19_12 field_100;
        s16    field_104;
        q3_12  field_106;
        q19_12 field_108; // Timer.
        q19_12 field_10C; // Timer.
        q19_12 distanceToPlayer;
        u8     field_114;
        u8     field_115;
        u8     field_116;
        s8     __pad_117;
        q3_12  field_118; // Angle.
        q3_12  field_11A; // Angle.
        s32    flags; /** `e_*Flags` */
        s32    field_120; // Kaufmann: set once his gunshot effect has started.
        q3_12  moveDistance_124;
        q3_12  moveSpeed;
    } s_PropsNpc;

    /** @brief Properties used by `Chara_PathWaypointExecute` while an NPC walks an event path.
     TODO: Most fields here likely overwrite existing properties on that NPC at same offset, should note down which ones.
    */
    typedef struct _PropsNpcPath
    {
        s32 controlState; /** `e_*Control` */
        s32 pathStep;
        s32 freeze;       /** `bool` */
        s32 field_F4;
        s32 resetControlState;
        s32 field_FC;     // Set alongside `controlState`.
        s8  unk_100[36];
        s32 field_124;    // `moveDistance_124` and `moveSpeed` together.
    } s_PropsNpcPath;

    /** @brief Air Screamer or Night Flutter character properties. */
    typedef struct _PropsAirScreamer
    {
        u32     field_E8_0  : 4; // `AirScreamerHit_None` step.
        u32     field_E8_4  : 4; /** `bool` */
        u32     field_E8_8  : 4;
        u32     __pad_E8_12 : 20;
        s32     field_EC;
        s16     field_F0; // } Maybe 2D offset like in Creeper properties? Must check.
        s16     field_F2; // }
        s32     field_F4;
        VECTOR3 targetPosition;     /** Q19.12 */
        VECTOR3 headTargetPosition; /** Q19.12 */
        VECTOR3 targetPosition2;    /** Q19.12 | TODO: Unknown purpose. */
        s32     flags; /** `e_AirScreamerFlags` */
        q19_12  timer_120;
        q19_12  floorHeight;
    } s_PropsAirScreamer;

    /** @brief Bloodsucker character properties. */
    typedef struct _PropsBloodsucker
    {
        q19_12 timer_E8;
        q19_12 timer_EC;
        q19_12 timer_F0;
        q19_12 timer_F4;
        s8     unused_F8[36]; /** @unused Probably explicit filler fields originally. */
        s32    flags;         /** `e_BloodsuckerFlags` */
    } s_PropsBloodsucker;

    /** @brief Cat character properties. */
    typedef struct _PropsCat
    {
        u8 field_E8; // `bool`
    } s_PropsCat;

    /** @brief Creeper character properties. */
    typedef struct _PropsCreeper
    {
        u16    flags; /** `e_CreeperFlags` */
        s8     __pad_EA[2];
        q3_12  collisionOffsetX;
        q3_12  collisionOffsetZ;
        q19_12 attackTimer;
        q19_12 targetPositionX;
        q19_12 targetPositionZ;
        q19_12 homePositionX;
        q19_12 homePositionZ;
        q19_12 chirpTimer;
        q3_12  angleToTarget;
        s16    animStatus_10A; // TODO: Purpose unclear.
        q4_12  moveSpeed;
    } s_PropsCreeper;

    /** @brief Floatstinger character properties. */
    typedef struct _PropsFloatstinger
    {
        s16        flags_E8;
        s8         unk_EA[2];
        q3_12      field_EC; // `vy` angle.
        q3_12      field_EE; // Angle.
        q19_12     field_F0; // Timer.
        q3_12      field_F4; // Angle.
        s8         __pad_F6[2];
        u16        field_F8;
        u16        field_FA;
        q4_12      field_FC; // Timer?
        s8         __pad_FE[2];
        q19_12     field_100; // Timer.
        u8         field_104;
        s8         __pad_105;
        s16        field_106;
        q19_12     field_108; // Damage related.
        q3_12      field_10C; // Angle, backup of `field_F4`?
        q3_12      field_10E; // Timer?
        s8         unk_110[24];
    } s_PropsFloatstinger;

    /** @brief Groaner character properties. */
    typedef struct _PropsGroaner
    {
        s16        flags; /** `e_GroanerFlags` */
        s8         __pad_EA[2];
        q3_12      targetHeadingAngle;
        q3_12      flexAngle;
        q3_12      field_F0; // } XZ offset?
        q3_12      field_F2; // }
        q19_12     targetPositionX;
        q19_12     targetPositionZ;
        q3_12      angleToTarget;
        q3_12      field_FE;
        u16        relKeyframeIdx_100;
        s8         __pad_102[2];
        q19_12     timer_104;
        u32        field_108;
        q3_12      timer_10C; // SFX timer?
        u8         field_10E; // } Sound states?
        u8         field_10F; // }
        u8         playLeftFootstepSfx;  /** `bool` */
        u8         playRightFootstepSfx; /** `bool` */
        s8         __pad_112[2];
        q3_12      field_114; // Move speed coefficient?
    } s_PropsGroaner;

    /** @brief Hanged Scratcher character properties. */
    typedef struct _PropsHangedScratcher
    {
        s16    flags; /** `e_HangedScratcherFlags` */
        q4_12  timer_EA;
        q3_12  targetHeadingAngle_EC;
        u8     field_EE;
        s8     __pad_EF;
        q3_12  offsetX_F0; // } Offsets passed to `Chara_MovementUpdate1`
        q3_12  offsetZ_F2; // }
        q19_12 positionX_F4;
        q19_12 positionZ_F8;
        q3_12  field_FC;
        s8     __pad_FE[2];
        q3_12  timer_100;
        u8     field_102;
        u8     field_103;
        s32    field_104;
        q3_12  timer_108;
        s8     __pad_10A[2];
        q4_12  radiusMax_10C; // } Used as `Chara_MoveSpeedUpdate` limit param, TODO: rename?
        q4_12  radiusMin_10E; // }
    } s_PropsHangedScratcher;

    /** @brief Incubus character properties, also used by Monster Incubator. */
    typedef struct _PropsIncubus
    {
        q19_12 timer_E8;
        s32    field_EC; // Flags?
        s32    someState_F0;
        q19_12 bossFightTimer_F4;
        s8     __pad_F8[48];
    } s_PropsIncubus;

    /** @brief Larval Stalker character properties. */
    typedef struct _PropsLarvalStalker
    {
        u16        flags_E8; /** `e_LarvalStalkerFlags` */
        u8         field_EA;
        s8         __pad_EB;
        q19_12     timer_EC;
        q20_12     timer_F0;
        s16        keyframeIdx_F4; // Relative keyframe?
        s16        keyframeIdx_F6; // Relative keyframe?
        q19_12     targetPositionX;
        q19_12     targetPositionZ;
        q3_12      angle_100;
        q3_12      angle_102;
        q19_12     animTime_104;
        q3_12      angle_108;
        q4_12      timer_10A;
        u_Property field_10C;
        VECTOR3    field_110;
        s32        flags_11C;
        u_Property field_120;
        s16        field_124;
        q3_12      moveSpeed;
    } s_PropsLarvalStalker;

    /** @brief Monster Cybil character properties. */
    typedef struct _PropsMonsterCybil
    {
        s32    controlState; /** `e_MonsterCybilControl` */
        s16    field_EC;
        s16    field_EE;          // Anim index (or full anim status?).
        q19_12 freeze;            // Timer.
        s32    field_F4;
        q19_12 resetControlState; // Timer.
        s8     __pad_FC[2];
        s16    field_FE;          // Index.
        q19_12 field_100;
        s16    field_104;
        q3_12  field_106;         // Angle or counter (`func_800D8D7C`)? May change usage depending on state step?
        q19_12 field_108;         // Timer.
        q19_12 field_10C;         // Timer.
        q19_12 distanceToPlayer;
        u8     field_114;
        u8     field_115;
        u8     field_116;
        s8     __pad_117;
        q3_12  field_118;         // Angle.
        q3_12  field_11A;         // Angle.
        s32    flags;
        s16    field_120;         // `bool`
        q3_12  field_122;         // Angle to player.
        q3_12  moveDistance_124;
        q3_12  moveSpeed;
    } s_PropsMonsterCybil;

    /** @brief Puppet Nurse or Puppet Doctor character properties. */
    typedef struct _PropsPuppetNurse
    {
        VECTOR3       position_E8; /** Q19.12 */
        s_CharaDamage damage;
        q19_12        field_104; // Timer.
        q19_12        field_108; // `vx` position?
        q19_12        field_10C; // `vz` position?
        q19_12        moveSpeed;
        q19_12        field_114; // Damage related.
        u8            field_118; // Used in switch, values 0 - 2.
        u8            modelVariantIdx;
        q3_12         field_11A; // Timer.
        q3_12         field_11C; // Heading angle.
        s16           field_11E; // `bool`
        q3_12         field_120;
        u16           flags_122; /** `e_PuppetNurseFlags` */
        s_800D5710*   field_124;
    } s_PropsPuppetNurse;

    /** @brief Romper character properties. */
    typedef struct _PropsRomper
    {
        s32    flags; /** `e_RomperFlags` */
        q3_12  angle_EC; // Target heading angle?
        s16    field_EE;
        q3_12  field_F0; // Move speed accumulation for this tick.
        q3_12  rotationY_F2;
        q19_12 field_F4; // Relative anim time?
        q3_12  movementOffsetX;
        q3_12  movementOffsetZ;
        q19_12 targetPositionX_FC;
        q19_12 targetPositionZ_100;
        s32    field_104;
        q19_12 positionX_108;
        u8     field_10C; // Relative keyframe index?
        s8     __pad_10D;
        u16    field_10E;
        q19_12 positionZ_110;
        u8     field_114;
        u8     field_115;
        q3_12  field_116;
        q3_12  timer_118;
        u8     field_11A;
        s8     __pad_11B;
        q3_12  timer_11C;
        s8     __pad_11E[2];
        q19_12 distance_120;
        q19_12 field_124; // Move speed step?
    } s_PropsRomper;

    /** @brief Split Head character properties. */
    typedef struct _PropsSplitHead
    {
        u16     flags; /** `e_SplitHeadFlags` */
        u8      field_EA;
        s8      __pad_EB;
        q3_12   angle_EC;
        q3_12   field_EE;
        q4_12   angle_F0;
        q4_12   timer_F2;
        q4_12   timer_F4;
        s8      __pad_F8[2];
        q19_12  animTime; // Set to `Q12(201.0f)` if not walking forward.
        s32     field_FC;
        s32     field_100;
        s8      unk_104[4];
        u8      field_108[4];
        q19_12  field_10C;
        VECTOR3 field_110;
        s32     flags_11C; /** `e_SplitHeadFlags` */
        s8      unk_120[4];
        s16     field_124;
        q3_12   moveSpeed;
    } s_PropsSplitHead;

    /** @brief Stalker character properties. */
    typedef struct _PropsStalker
    {
        s16    flags; /** `e_StalkerFlags` */
        s8     __pad_EA[2];
        q3_12  offset_EC;
        q3_12  offset_EE;
        q19_12 targetPositionX;
        q19_12 targetPositionZ;
        q19_12 timer_F8;
        s16    keyframeIdx_FC;    // Or anim status?? Seems to be used as both.
        s16    relKeyframeIdx_FE; // Unsure.
        q3_12  targetHeadingAngle;
        s16    sfxId_102;
        q19_12 relAnimTime_104;
        q4_12  timer_108;
        u8     field_10A;
        s8     __pad_10B;
        q19_12 timer_10C;
        q19_12 health_110;
        q3_12  angle_114;
        q4_12  timer_116;
    } s_PropsStalker;

    /** @brief Twinfeeler character properties. */
    typedef struct _PropsTwinfeeler
    {
        // TODO: Weird `field_E8` access.
        u_Property    field_E8;
        //q3_12         sfxTimer_E8;
        //q4_12         field_EA;
        s_CharaDamage damage;
        q19_12        digTimer;
        q19_12        spawnPositionX; /** @unused */
        q19_12        spawnPositionZ; /** @unused */
        q19_12        prevMoveSpeed;
        q19_12        accumulatedDamage;
        s16           field_110; /** @unused */
        s8            __pad_112[2];
        u32           flags;     /** `e_TwinfeelerFlags` */
        u16           field_118; /** `bool` */
        s8            __pad_11A[2];
        q19_12        prevHealth;
        s8            __pad_120[8];
    } s_PropsTwinfeeler;
}

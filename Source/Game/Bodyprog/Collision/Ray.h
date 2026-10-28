#pragma once

#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Formats/Ipd.h"

namespace Silent::Game
{
    typedef struct
    {
        s8         unk_0[8];
        DVECTOR    field_8;
        s16        field_C;
        s16        field_E;
        q19_12     field_10; // } X
        q19_12     field_14; // } Z
        s16        field_18;
        s8         unk_1A[2];
        s32        field_1C; // Index into `field_20`.
        DVECTOR_XZ field_20[2];
    } s_func_8006E490;

    // Something with a projected character position.
    typedef struct
    {
        /* 0x0 */ q23_8 positionX;
        /* 0x4 */ q23_8 positionZ;
        /* 0x8 */ q7_8  floorHeight;
        /* 0xA */ q7_8  topHeight; // Ceiling height?
        /* 0xC */ q7_8  field_C;   // Collision cylinder radius?
        /* 0xE */ s16   field_E;
    } s_RayState_6C;

    // Something related to subcell ranges.
    typedef struct
    {
        s16 field_0;
        s16 field_2;
    } s_RayState_8C;

    /** @brief State for an in-progress ray trace. Contains pointers to active characters among other things. */
    struct s_RayState
    {
        bool             field_0; // "Use cylinder"?
        s16              field_4; // Collision flags.
        q7_8             field_6;
        q7_8             field_8; // Hit distance? `SHRT_MAX` if no valid hit.
        s8               __pad_A[2];
        VECTOR3          field_C; // Q19.12
        s8               unk_18[4];
        q7_8             floorHeight;
        s8               __pad_1E[2];
        s_SubCharacter*  field_20;
        q7_8             field_24;   // X offset.
        q7_8             field_26;   // Z offset.
        s32              floorType; /** `e_FloorType` */
        VECTOR3          from;       // Q23.8
        s8               unk_38[4];
        s32              field_3C; // X  } Q23.8 `VECTOR3`?
        s32              field_40; // Y? }
        s32              field_44; // Z  }
        s8               unk_48[4];
        q7_8             field_4C; // X?
        q7_8             field_4E; // Height offset.
        SVECTOR          offset;   // Q23.8
        u16              field_58; // X
        s16              field_5A; // Z
        q7_8             rayDistance;
        s16              field_5E;
        s16              field_60;
        s8               __pad_62[2];
        s_SubCharacter** collCharas;
        s32              collCharaCount;
        s_RayState_6C    field_6C;
        s32              field_7C;
        s32              field_80;
        u16              field_84;
        u8               __pad_86[2];
        s32              field_88;    // `field_8C` size.
        s_RayState_8C    field_8C[1]; // Unknown buffer size.
    };

    /** @brief Ray trace line of sight info. */
    struct s_RayTrace
    {
        s8              hasHit;     /** `bool` */
        u8              floorType; /** `e_FloorType` */
        s8              __pad_2[2];
        VECTOR3         target; /** Q19.12 */
        s_SubCharacter* character;
        q19_12          hitDistance;
        q19_12          floorHeight;
        q3_12           field_1C; // Angle.
    };

    bool Ray_TraceQuery(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* to);

    /** Ray function. */
    bool Ray_CharaTraceQuery(s_RayTrace* trace, const VECTOR3* from, VECTOR3* offset, s_SubCharacter* excludedChara);

    void Ray_MissSet(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset, q23_8 dist);

    /** @brief Checks if an obstruction in a ray's line of sight has been hit, ignoring a specified character.
     *
     * @param trace Ray trace.
     * @param from Ray origin.
     * @param offset Ray target offset from origin.
     * @param excludedChara Character to exclude.
     * @return `true` if there is an obstruction, `false` otherwise.
     */
    bool Ray_LosHitCheck(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset, s_SubCharacter* excludedChara);

    bool func_8006DC18(s_RayTrace* trace, const VECTOR3* from, const VECTOR3* offset);

    bool Ray_TraceSetup(s_RayState* state, bool useCylinder, q7_8 radiusOffset, const VECTOR3* from, const VECTOR3* offset, q19_12 arg5, q19_12 arg6,
                        s_SubCharacter** collCharas, s32 collCharaCount);

    bool Ray_TraceRun(s_RayTrace* trace, s_RayState* state);

    // Fills `state` with info.
    void func_8006E0AC(s_RayState* state, const s_IpdCollisionData* collData);

    void func_8006E150(s_func_8006E490* arg0, DVECTOR arg1, DVECTOR arg2);

    void func_8006E490(s_func_8006E490* arg0, u32 flags, q19_12 posX, q19_12 posZ);

    void func_8006E53C(s_RayState* state, s_IpdShapeRange* shapeRangePair, s_IpdCollisionData* collData);

    void func_8006E78C(s_RayState* state, s_IpdPolygon* polygon, SVECTOR3* splitVerts, s_IpdSurface* surfaces, bool hasSurface);

    void func_8006EB8C(s_RayState* state, s_IpdCylinder* cylinder);

    // Fills ray hit data?
    void func_8006EE0C(s_RayState_6C* arg0, bool useCylinder, const s_SubCharacter* chara);

    // Sets hit character ray state?
    void func_8006EEB8(s_RayState* state, s_SubCharacter* chara);
}

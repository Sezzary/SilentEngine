#pragma once

#include "Psx.h"

namespace Silent::Game
{
    /** @brief Geometry primitive. */
    struct s_Primitive
    {
        u16 field_0;
        u16 field_2;
        u16 field_4;
        union
        {
            struct
            {
                u8 field_6_0     : 8; // Set to `s_Material::field_E`.
                s8 materialIdx   : 7;
                u8 isTransparent : 1; /** `bool` */
            } bits;
        u16 flags; // @hack `func_8005AC50` accesses `isTransparent` above with some weird shifts, haven't found how to make it work with bitfield yet.
        } field_6;
        u16 field_8;
        u16 field_A;
        u8  field_C[4];  // } Unknown type. `func_8005AC50` reads these as `s32`, but that breaks other funcs which use this struct. `s32` reads might have just been `memcpy`?
        u8  field_10[4]; // }
    };

    /** @brief Mesh data header. */
    struct s_MeshHeader
    {
        u8           primitiveCount;
        u8           vertexCount;
        u8           normalCount;
        u8           unkCount_3;
        s_Primitive* primitives;
        DVECTOR*     verticesXy;
        s16*         verticesZ;
        s_Normal*    normals;
        u8*          unkPtr_14;
    };

    /** @brief Model data header. */
    struct s_ModelHeader
    {
        u_Filename    name;
        u8            meshCount;
        u8            vertexOffset;
        u8            normalOffset;
        u8            field_B_0 : 1;
        u8            field_B_1 : 3; // Value used in `func_800571D0` switch.
        u8            field_B_4 : 2;
        u8            unk_B_6   : 2;
        s_MeshHeader* meshHdrs;
    };

    /** @brief Model info. */
    struct s_ModelInfo
    {
        s32            field_0; // Bone flags?
        GsCOORDINATE2* coord;
        s_ModelHeader* modelHdr;
        s32            modelIdx;
    };
}

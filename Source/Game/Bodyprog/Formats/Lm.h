#pragma once

#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Bodyprog/Formats/Texture.h"

namespace Silent::Game
{
    constexpr char LM_HEADER_MAGIC = '0';
    constexpr int  LM_VERSION      = 6;

    /** @brief LM file material. */
    struct s_Material
    {
        u_Filename name;
        s_Texture* texture;
        u8         field_C;
        u8         unk_D[1];
        u8         field_E;
        u8         field_F;
        u16        field_10;
        u16        field_12;
        union
        {
            u8  u8[2];
            u16 u16;
        } field_14;
        union
        {
            u8  u8[2];
            u16 u16;
        } field_16;
    };

    /** @brief LM file header. */
    struct s_LmHeader
    {
        u8             magic;    /** Equal to `LM_HEADER_MAGIC` in valid file. */
        u8             version;  /** Equal to `LM_VERSION` in valid file. */
        u8             isLoaded; /** `bool` */
        u8             materialCount;
        s_Material*    materials;
        u8             modelCount;
        s8             __pad_9[3];
        s_ModelHeader* modelHdrs;
        u8*            modelOrder;
    };
}

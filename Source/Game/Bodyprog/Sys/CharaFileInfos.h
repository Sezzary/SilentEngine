#pragma once

namespace Silent::Game
{
    struct FsImageDesc;

    /** @brief Character file info.
     * Holds file IDs of anim/model/texture for each `e_CharaId` along with some data used in VC camera code.
     */
    struct s_CharaFileInfo
    {
        s16            animFileIdx;
        s16            modelFileIdx;
        s16            textureFileIdx    : 16;
        q8_8           field_6           : 10;
        u16            materialBlendMode : 6; /** `e_BlendMode` */
        s_FsImageDesc* field_8;               // TODO: Extra texture pointer? Usually `NULL` in `CHARA_FILE_INFOS`.
        u16            cameraAnchor  : 2;     /** `e_CameraAnchor` */
        q19_12         cameraOffsetY : 14;
        // 2 bytes of padding.
    };

    /** Array containg file IDs used for each `e_CharaId`, used in `Fs_QueueStartReadAnm`. */
    extern s_CharaFileInfo CHARA_FILE_INFOS[Chara_Count]; // 0x800A90FC
}

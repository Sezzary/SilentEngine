#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Sys/SettingsReset.h"

#include "Game/Bodyprog/Screen/Draw.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"

namespace Silent::Game
{
    void Settings_ScreenAndVolUpdate() // 0x800333CC
    {
        Screen_XyPositionSet(g_GameWork.config.screenPositionX, g_GameWork.config.screenPositionY);
        SD_Call((g_GameWork.config.soundType != 0) ? AudioMode_Mono : AudioMode_Stereo);
        //Sd_SetVolume(OPT_SOUND_VOLUME_MAX, g_GameWork.config.volumeBgm, g_GameWork.config.volumeSe);
    }

    void Settings_RestoreDefaults() // 0x8003342C
    {
        g_GameWork.config.extraWeaponCtrl = 1;
        g_GameWork.config.brightness      = 3;

        Settings_RestoreControlDefaults(0);

        g_GameWork.config.vibrationEnabled = OPT_VIBRATION_ENABLED;
        g_GameWork.config.volumeBgm        = OPT_SOUND_VOLUME_MAX;
        g_GameWork.config.volumeSe         = OPT_SOUND_VOLUME_MAX;

        Settings_ScreenAndVolUpdate();

        g_GameWork.config.extraBloodColor = 0;
    }

    void Settings_RestoreControlDefaults(s32 configIdx) // 0x80033480
    {
        u32  i;
        u16* ptr;

        static const s_ControllerConfig DEFAULT_CONTROLLER_CONFIGS[3] =
        {
            {
                .enter     = ControllerFlag_Start | ControllerFlag_Cross,
                .cancel    = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
                .skip      = ControllerFlag_Start,
                .action    = ControllerFlag_Cross,
                .aim       = ControllerFlag_R2,
                .light     = ControllerFlag_Circle,
                .run       = ControllerFlag_Square,
                .view      = ControllerFlag_L2,
                .stepLeft  = ControllerFlag_L1,
                .stepRight = ControllerFlag_R1,
                .pause     = ControllerFlag_Start,
                .item      = ControllerFlag_Select,
                .map       = ControllerFlag_Triangle,
                .option    = ControllerFlag_None
            },
            {
                .enter     = ControllerFlag_Start | ControllerFlag_Cross,
                .cancel    = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
                .skip      = ControllerFlag_Start,
                .action    = ControllerFlag_Cross,
                .aim       = ControllerFlag_R1,
                .light     = ControllerFlag_Circle,
                .run       = ControllerFlag_Square,
                .view      = ControllerFlag_L1,
                .stepLeft  = ControllerFlag_L2,
                .stepRight = ControllerFlag_R2,
                .pause     = ControllerFlag_Start,
                .item      = ControllerFlag_Select,
                .map       = ControllerFlag_Triangle,
                .option    = ControllerFlag_None
            },
            {
                .enter     = ControllerFlag_Start | ControllerFlag_Cross,
                .cancel    = ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Square,
                .skip      = ControllerFlag_Start,
                .action    = ControllerFlag_Cross,
                .aim       = ControllerFlag_R2,
                .light     = ControllerFlag_Circle,
                .run       = ControllerFlag_Square,
                .view      = ControllerFlag_L2,
                .stepLeft  = ControllerFlag_L1,
                .stepRight = ControllerFlag_R1,
                .pause     = ControllerFlag_Start,
                .item      = ControllerFlag_Triangle,
                .map       = ControllerFlag_Select,
                .option    = ControllerFlag_None
            }
        };

        for (i = 0, ptr = (u16*)&g_GameWorkPtr->config.controllerConfig; i < INPUT_ACTION_COUNT; i++, ptr++)
        {
            *ptr = (&DEFAULT_CONTROLLER_CONFIGS[configIdx].enter)[i];
        }
    }
}

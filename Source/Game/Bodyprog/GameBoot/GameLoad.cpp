#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/GameBoot/GameLoad.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Sys/Demo.h"
#include "Game/Bodyprog/Events/GameSysStates.h"
#include "Game/Bodyprog/Events/Radio.h"
#include "Game/Bodyprog/GameBoot/GameBoot.h"
//#include "Game/Bodyprog/item_screens.h"
//#include "Game/Bodyprog/player.h"
//#include "Game/Bodyprog/ranking.h"
#include "Game/Bodyprog/Screen/Background.h"
#include "Game/Bodyprog/Screen/Data.h"
#include "Game/Bodyprog/Screen/Draw.h"
#include "Game/Bodyprog/Screen/Fade.h"
//#include "Game/Bodyprog/sound_background.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Main/Mem.h"
#include "Game/Main/Rng.h"
#include "Game/Screens/Stream/Stream.h"

// nps_anims_clear.c

namespace Silent::Game
{
    void Anim_CharaTypeAnimInfoClear(void) // 0x800348C0
    {
        //bzero(&g_CharaModelAnimsData[1], 72);
    }
}

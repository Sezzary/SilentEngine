#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/GameBoot/GameBoot.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Main/FsQueue.h"
//#include "Game/Bodyprog/player_logic.h"

namespace Silent::Game
{
    void GameBoot_SavegameInitialize(s8 overlayId, s32 difficulty) // 0x800350BC
    {
        s32  i;
        s32* mapEnemyStatesPtr;

        bzero(g_SavegamePtr, sizeof(s_Savegame));

        g_SavegamePtr->mapIdx = overlayId;

        difficulty = CLAMP(difficulty, GameDifficulty_Easy, GameDifficulty_Hard);

        mapEnemyStatesPtr = g_SavegamePtr->mapEnemyStates;

        g_SavegamePtr->gameDifficulty = difficulty;
        g_SavegamePtr->paperMapIdx     = PaperMapIdx_OldTown;

        // Defines all enemies from an overlay as alive.
        // Odd code. Possibly a hack.
        for (i = 0; i < 45; i++)
        {
            mapEnemyStatesPtr[44] = NO_VALUE;
            mapEnemyStatesPtr--;
        }

        //Game_SavegameResetPlayer();
    }

    void GameBoot_PlayerInit() // 0x80035178
    {
        //WorldGfx_MapInit();
        //CharaModel_AllModelsFree();
        //Item_HeldItemModelFree();
        //Anim_BoneInit(FS_BUFFER_0, g_SysWork.playerBoneCoords); // Load player anim file?
        //WorldGfx_PlayerModelProcessLoad();

        g_SysWork.unused_229C = NO_VALUE;

        if ((g_SavegamePtr->itemToggleFlags >> 1) & (1 << 0)) // `& ItemToggleFlag_FlashlightOff`
        {
            //Game_TurnFlashlightOff();
        }
        else
        {
            //Game_TurnFlashlightOn();
        }

        //g_CharaTypeAnimInfo[0].animBufferSize2_10 = 0x2E630;
        //g_CharaTypeAnimInfo[0].animBufferSize1_C  = 0x2E630;
        //Game_PlayerInfoInit();
    }

    void GameBoot_MapLoad(s32 mapIdx) // 0x8003521C
    {
        Fs_QueueStartRead((e_FsFile)((int)FILE_VIN_MAP0_S00_BIN + mapIdx));
        //Map_EffectTexturesLoad(mapIdx);
        //GameFs_PlayerMapAnimLoad(mapIdx);

        // If the player spawns in the map with a weapon equipped (either because it's a demo
        // or because the player saved the game with a weapon equipped), this and the next function
        // make it appear and allocate its data.
        if (g_SysWork.processFlags & (ProcessFlag_NewGame | ProcessFlag_LoadSave |
                                           ProcessFlag_Continue | ProcessFlag_BootDemo))
        {
            //WorldGfx_PlayerPrevHeldItem(&g_SysWork.playerCombat);
        }

        //Gfx_PlayerHeldItemAttach(g_SysWork.playerCombat.weaponAttack);
    }
}

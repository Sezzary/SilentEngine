#include "Framework.h"
#include "Game/Bodyprog/Events/PlayerPosUpdate.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Map/Map.h"

namespace Silent::Game
{
    void Game_PlayerMapPositionSet(s_MapPoint2d* mapPoint) // 0x800371E8
    {
        auto& player = g_SysWork.playerWork.player;

        q19_12 headingAngle = Q12_ANGLE_FROM_Q8(mapPoint->triggerParam0);
        Math_SVectorSet(&player.rotation, Q12_ANGLE(0.0f), headingAngle, Q12_ANGLE(0.0f));

        player.position.vy = Q12(0.0f);
        player.position.vx = mapPoint->positionX;
        player.position.vz = mapPoint->positionZ;

        if (mapPoint->triggerParam1 >= 2)
        {
            player.position.vx += Q12_MULT_FLOAT_PRECISE(Math_Sin(headingAngle), 0.4f);
            player.position.vz += Q12_MULT_FLOAT_PRECISE(Math_Cos(headingAngle), 0.4f);
        }

        g_SysWork.loadingScreenIdx = mapPoint->loadingScreenId;

        if (mapPoint->paperMapIdx == PaperMapIdx_24)
        {
            g_SavegamePtr->paperMapIdx = PaperMapIdx_OtherPlaces;
        }
        else if (mapPoint->paperMapIdx != PaperMapIdx_OtherPlaces)
        {
            g_SavegamePtr->paperMapIdx = mapPoint->paperMapIdx;
        }

        g_SysWork.cameraAngleY = headingAngle;

        //func_8007E9C4();
        //Game_MapRoomIdxUpdate();
    }

    void Game_PlayerHeightUpdate(void) // 0x80037334
    {
        if (g_MapOverlayHdr.updateWorldObjects != NULL)
        {
            g_MapOverlayHdr.updateWorldObjects();
        }

        auto surface = s_CollisionSurface{};
        //Collision_SurfaceGet(&surface, g_SysWork.playerWork.player.position.vx, g_SysWork.playerWork.player.position.vz);
        g_SysWork.playerWork.player.position.vy = surface.floorHeight;
    }
}

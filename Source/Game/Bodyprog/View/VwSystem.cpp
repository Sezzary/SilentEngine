#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/View/VwSystem.h"

#include "Game/Bodyprog/Screen/Data.h"
#include "Game/Bodyprog/View/Enums.h"
#include "Game/Bodyprog/View/Structs.h"
#include "Game/Bodyprog/View/VcMain.h"
#include "Game/Bodyprog/View/VcUtil.h"
#include "Game/Bodyprog/View/VwCalc.h"
#include "Game/Bodyprog/View/VwMain.h"

namespace Silent::Game
{
    void Vc_CurNearRoadSet(VC_WORK* work, VC_NEAR_ROAD_DATA* road)
    {
        work->cur_near_road = *road;
    }

    void Vc_FlagClear(s32 flags)
    {
        vcWork.flags &= ~flags;
    }

    void Vc_FlagSet(s32 flags)
    {
        vcWork.flags |= flags;
    }
}

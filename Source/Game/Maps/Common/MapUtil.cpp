#include "Framework.h"
#include "Game/Maps/Common/MapUtil.h"

#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Savegame.h"

namespace Silent::Game
{
    #define CheckRange(axis, low, high)    \
    ({                                     \
        s32 val = (axis);                  \
        (low) <= (val) && (val) <= (high); \
    })

    #define CheckNotInRange(axis, low, high)  \
    ({                                        \
        s32 val = (axis);                     \
        !((low) <= (val) && (val) <= (high)); \
    })

    static inline u32 GetXIdx(q19_12 posX, q19_12 posZ = Q12(0.0f))
    {
        if (g_SavegamePtr->mapIdx == MapIdx_MAP5_S01)
        {
            if (CheckRange(posX + Q12(38.5f), Q12(-16.5f), Q12(16.5f)) && posZ > Q12(-160.0f))
            {
                if (posZ < Q12(-120.0f) || (posZ < Q12(-24.0f) && posX < Q12(-32.0f)) ||
                    CheckRange(posX + Q12(40.0f), Q12(-12.0f), Q12(12.0f)))
                {
                    return 1;
                }
            }

            if (CheckRange(posX + Q12(-38.0f), Q12(-6.0f), Q12(6.0f)) && posZ < Q12(-40.0f))
            {
                return 2;
            }

            return 0;
        }
        else
        {
            if (CheckRange(posX + Q12(240.0f), Q12(-24.0f), Q12(24.0f)))
            {
                return 1;
            }

            if (CheckRange(posX + Q12(120.0f), Q12(-24.0f), Q12(24.0f)))
            {
                return 2;
            }

            if (CheckRange(posX + Q12(16.0f), Q12(0.0f), Q12(40.0f)))
            {
                return 3;
            }

            if (CheckRange(posX - Q12(120.0f), Q12(-24.0f), Q12(24.0f)))
            {
                return 4;
            }

            return 0;
        }
    }

    static inline s32 GetYIdx(q19_12 posX, q19_12 posY)
    {
        if (CheckRange(posY + Q12(202.0f), Q12(-18.0f), Q12(18.0f)))
        {
            return 1;
        }

        if (CheckRange(posY + Q12(79.0f), Q12(-15.0f), Q12(15.0f)) && posX < Q12(160.0f))
        {
            return 2;
        }

        if (CheckRange(posY + Q12(16.0f), 0, Q12(32.0f)))
        {
            return 3;
        }

        if (CheckRange(posY - Q12(124.0f), Q12(-20.0f), Q12(20.0f)))
        {
            return 4;
        }

        return 0;
    }

    u8 Map_RoomIdxGet(q19_12 posX, q19_12 posZ)
    {
        if (g_SavegamePtr->mapIdx == MapIdx_MAP6_S04 ||
            g_SavegamePtr->mapIdx == MapIdx_MAP6_S05)
        {
            s32 result;

            if (posZ > Q12(80.0f))
            {
                result = 5;
            }
            else if (Q12_SQUARE_PRECISE(posX + Q12(16.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(64.0f)) < Q12(SQUARE(18)))
            {
                result = 3;
            }
            else if (Q12_SQUARE_PRECISE(posX + Q12(-14.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(28.0f)) < Q12(SQUARE(18)))
            {
                result = 2;
            }
            else if (Q12_SQUARE_PRECISE(posX + Q12(14.0f)) + Q12_SQUARE_PRECISE(posZ + Q12(28.0f)) < Q12(SQUARE(5)))
            {
                result = 4;
            }
            else
            {
                result = 1;
            }

            return result;
        }
        else if (g_SavegamePtr->mapIdx == MapIdx_MAP6_S03)
        {
            s32 ret;

            if (posX < Q12(27.0f))
            {
                ret = 17;
                if (posX <= Q12(13.0f))
                {
                    ret = 15;
                    if (posX > Q12(-13.0f))
                    {
                        ret = 16;
                    }
                }
            }
            else if (posX < Q12(56.0f))
            {
                ret = 18;
            }
            else
            {
                if (posX < Q12(72.0f))
                {
                    ret = 19;
                }
                else
                {
                    ret = 20;
                }
            }

            return ret;
        }
        else
        {
            extern u8 MAP_ROOM_IDXS[];

            s32 xIdx;
            s32 yIdx;
            u8  res = 0;

            bool hasSecondaryGrid = g_SavegamePtr->mapIdx == MapIdx_MAP0_S00 ||
                                    g_SavegamePtr->mapIdx == MapIdx_MAP0_S01 ||
                                    g_SavegamePtr->mapIdx == MapIdx_MAP2_S00 ||
                                    g_SavegamePtr->mapIdx == MapIdx_MAP2_S03;

            if (hasSecondaryGrid)
            {
                extern u8 sharedData_800DF2DC_0_s00[];

                // Check if coordinates are within primary grid bounds.
                if (posX < Q12(-320.0f) || posX >= Q12(240.0f) ||
                    posZ < Q12(-240.0f) || posZ >= Q12(200.0f))
                {
                    res = 0;
                }
                else
                {
                    // Determine XY indices in primary grid.
                    xIdx = GetXIdx(posX);
                    yIdx = GetYIdx(posX, posZ);

                    // @todo Not migrated in decomp yet.
                    //res  = sharedData_800DF2DC_0_s00[(xIdx * 5) + yIdx];
                }
            }
            else if (g_SavegamePtr->mapIdx == MapIdx_MAP2_S02 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP4_S00 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP4_S02 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP4_S03 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP4_S05 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP4_S06)
            {
                if (CheckRange(posX + Q12(120.0f), Q12(-14.0f), Q12(14.0f)) && posZ < Q12(40.0f))
                {
                    xIdx = 1;
                }
                else if (CheckRange(posX + Q12(60.0f), Q12(-9.0f),  Q12(9.0f)) &&
                         CheckRange(posZ + Q12(40.0f), Q12(-34.0f), Q12(34.0f)))
                {
                    xIdx = 2;
                }
                else if (CheckRange(posX + Q12(0.0f), Q12(-14.0f), Q12(14.0f)) && posZ > Q12(-80.0f))
                {
                    xIdx = 3;
                }
                else
                {
                    xIdx = 0;
                }

                if (CheckNotInRange(posX + Q12(40.0f), Q12(-120.0f), Q12(120.0f)))
                {
                    yIdx = 0;
                }
                else if (CheckRange(posZ + Q12(80.0f), Q12(-14.0f), Q12(14.0f)))
                {
                    yIdx = 1;
                }
                else if (CheckRange(posZ + Q12(0.0f), Q12(-14.0f), Q12(14.0f)))
                {
                    yIdx = 2;
                }
                else
                {
                    yIdx = 0;
                }

                //res = sharedData_800ED430_2_s02[(xIdx * 3) + yIdx];
            }
            else if (g_SavegamePtr->mapIdx == MapIdx_MAP5_S00)
            {
                res = 0;

                if (posX >= Q12(-40.0f))
                {
                    if (CheckRange(posX, Q12(16.0), Q12(24.0)) && posZ > Q12(-8.0))
                    {
                        res = (posZ < Q12(48.0)) ? 2 : 1;
                    }
                    else if (CheckRange(posZ + Q12(12.0), Q12(-4.0), Q12(4.0)) && CheckRange(posX - Q12(40.0), Q12(-8.0), Q12(8.0)))
                    {
                        res = 3;
                    }
                    else if (CheckRange(posX, Q12(48.0), Q12(56.0)) && CheckRange(posZ + Q12(40.0), Q12(-8.0), Q12(8.0)))
                    {
                        res = 4;
                    }
                }
                else if (CheckRange(posX, Q12(-64.0f), Q12(-56.0f)))
                {
                    if (posZ > Q12(-8.0))
                    {
                        res = 9;
                    }
                    else if (CheckRange(posZ + Q12(52.0), Q12(-36.0), Q12(36.0)))
                    {
                        res = 8;
                    }
                }
                else if (posX > Q12(-64.0) && posZ < Q12(-88.0))
                {
                    res = 10;
                }
                else if (CheckRange(posZ, Q12(-94.0), Q12(-90.0)) && posX > Q12(-89.0))
                {
                    res = 11;
                }
                else if (CheckRange(posX, Q12(-96.0), Q12(-88.0)) && CheckRange(posZ + Q12(46.0), Q12(-30.0), Q12(30.0)))
                {
                    res = 13;
                }
            }
            else if (g_SavegamePtr->mapIdx == MapIdx_MAP5_S01)
            {
                xIdx = GetXIdx(posX, posZ);

                // TODO: Could this be a `GetYIdx` inline like one above? No luck changing it yet.
                yIdx = 0;
                if (CheckRange(posZ + Q12(163.0f), Q12(-18.0f), Q12(18.0f)))
                {
                    yIdx = 1;
                }
                else if (posX > Q12(-40.0f))
                {
                    if (posX > Q12(40.0f))
                    {
                        if (CheckRange(posZ + Q12(36.0f), Q12(-16.0f), Q12(16.0f)))
                        {
                            yIdx = 2;
                        }
                    }
                    else if (CheckRange(posZ + Q12(43.5f), Q12(-12.0f), Q12(12.0f)))
                    {
                        yIdx = 2;
                    }
                }

                // @todo Not migrated in decomp yet.
                //res = sharedData_800ED430_2_s02[(xIdx * 3) + yIdx];
            }
            else if (g_SavegamePtr->mapIdx == MapIdx_MAP6_S00 ||
                     g_SavegamePtr->mapIdx == MapIdx_MAP6_S02)
            {
                res = 0;

                // @hack Weird temp vars needed for match.
                yIdx = Q12(-127.0f);
                if (CheckRange(posZ + yIdx, Q12(-17.0f), Q12(17.0f)) &&
                    ((yIdx = posX + Q12(192.0f), xIdx = Q12(8.0f) - 1, (u32)yIdx > xIdx) || posZ < Q12(130.0f)) &&
                    (posX < Q12(-166.0f) || posZ > Q12(112.0f)))
                {
                    res = 3;
                }
                else if (CheckRange(posZ + Q12(-8.5f), Q12(-31.5f), Q12(31.5f)) && posX < Q12(-159.5f))
                {
                    res = 16;
                }
                else if (posZ < Q12(0.0f) && posX < Q12(-80.0f))
                {
                    // @todo Migrate collision code.
                    //xIdx = Collision_FloorHeightGet(posX, posZ);

                    if (posX < Q12(-160.0f))
                    {
                        res = (xIdx > Q12(1.0f)) ? 17 : 18;
                    }
                    else if (posX >= Q12(-148.0f))
                    {
                        res = (xIdx > Q12(1.0f)) ? 18 : 19;
                    }
                    else
                    {
                        res = 18;
                    }
                }
            }

            // If no match in primary grid, try fallback grid.
            if (res == 0)
            {
                #define MAP_ROOM_STRIDE_Z ((MAP_ROOM_MAX_Z - MAP_ROOM_MIN_Z) / Q12(CHUNK_SIZE))

                // @todo MIN/MAX constants are unique for each map.
                /*if (posX < MAP_ROOM_MIN_X || posX >= MAP_ROOM_MAX_X ||
                    posZ < MAP_ROOM_MIN_Z || posZ >= MAP_ROOM_MAX_Z)
                {
                    // TODO: Not sure why these two maps need this.
                    #if defined(MAP6_S00) || defined(MAP6_S02)
                        return 0;
                    #else
                        res = 0;
                    #endif
                }
                else
                {
                    posX = (posX - MAP_ROOM_MIN_X) / Q12(CHUNK_SIZE);
                    posZ = (posZ - MAP_ROOM_MIN_Z) / Q12(CHUNK_SIZE);

                    res = MAP_ROOM_IDXS[(posX * MAP_ROOM_STRIDE_Z) + posZ];
                    if (hasSecondaryGrid)
                    {
                        if (res == 0)
                        {
                            res = 26;
                        }
                    }
                }*/
            }

            return res;
        }
    }
}

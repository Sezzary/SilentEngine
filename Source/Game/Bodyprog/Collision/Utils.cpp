#include "Framework.h"
#include "Game/Bodyprog/Collision/Utils.h"

#include "Game/Bodyprog/Collision/Collision.h"

namespace Silent::Game
{
    s_ActiveCollisionTriggers g_ActiveCollisionTriggers = {};

    void Collision_Init(void)
    {
        Collision_FlagsSet(CollisionTriggerFlag_Map);
        g_ActiveCollisionTriggers.collisionTriggerCount = 0;
    }

    int Collision_FlagsGet(void)
    {
        return g_ActiveCollisionTriggers.flags;
    }

    void Collision_FlagsSet(int collFlags)
    {
        g_ActiveCollisionTriggers.flags = collFlags;
    }

    void Collision_FlagBitsSet(int collFlags)
    {
        g_ActiveCollisionTriggers.flags |= collFlags;
    }

    void Collision_FlagBitsClear(int collFlags)
    {
        g_ActiveCollisionTriggers.flags = (g_ActiveCollisionTriggers.flags & ~collFlags) | CollisionTriggerFlag_Map;
    }

    void Collision_NearbyTriggersGet(q19_12 posX, q19_12 posZ, const s_CollisionTrigger* triggers)
    {
        constexpr q19_12 AABB_BUFFER = Q12(16.0f);

        // Run through collision triggers.
        for (const auto* curTrigger = triggers; !curTrigger->isEndOfArray; curTrigger++)
        {
            // Define AABB bounds.
            q19_12 minX = Q12(curTrigger->positionX);
            q19_12 maxX = Q12(curTrigger->positionX + curTrigger->sizeX);
            q19_12 minZ = Q12(curTrigger->positionZ);
            q19_12 maxZ = Q12(curTrigger->positionZ + curTrigger->sizeZ);

            // Expand AABB.
            minX -= AABB_BUFFER;
            maxX += AABB_BUFFER;
            minZ -= AABB_BUFFER;
            maxZ += AABB_BUFFER;

            // Collect collided trigger.
            if (posX >= minX && maxX >= posX &&
                posZ >= minZ && maxZ >= posZ)
            {
                g_ActiveCollisionTriggers.collisionTriggers[g_ActiveCollisionTriggers.collisionTriggerCount] = curTrigger;
                g_ActiveCollisionTriggers.collisionTriggerCount++;
            }
        }
    }
}

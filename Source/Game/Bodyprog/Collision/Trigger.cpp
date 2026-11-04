#include "Framework.h"
#include "Game/Bodyprog/Collision/Trigger.h"

#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/View/VcMain.h"
#include "Game/Bodyprog/View/VwMain.h"

namespace Silent::Game
{
    constexpr q19_12 DEFAULT_TRIGGER_HEIGHT = SHRT_MIN * 2;

    static auto g_TriggerIntersectCache = s_CollisionTriggerIntersect{};

    /** @brief Computes a trigger height from half-meter height steps.
     *
     * @note The trigger height has a starting offset of `Q12(-1.5f)`.
     *
     * @param steps Half-meter height steps.
     * @return Trigger height.
     */
    constexpr q19_12 TRIGGER_HEIGHT_GET(int steps)
    {
        return (-Q12(steps) / 2) - Q12(1.5f);
    }

    void Collision_TriggerQuery(q19_12* result, q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ)
    {
        auto* intersect = &g_TriggerIntersectCache;

        // Initialize line intersection.
        Collision_TriggerIntersectInit(intersect, posX, posZ, offsetX, offsetZ);

        // Collide active triggers.
        for (int i = 0; i < g_ActiveCollisionTriggers.collisionTriggerCount; i++)
        {
            if (Collision_TriggerIntersectGet(intersect, g_ActiveCollisionTriggers.collisionTriggers[i]))
            {
                break;
            }
        }

        // Set result.
        if (intersect->intersectAlpha == Q12(1.0f))
        {
            result[0] = Q12(32.0f);
            result[1] = Q12(DEFAULT_CEILING_HEIGHT);
        }
        else
        {
            result[0] = Math_MulFixed(Vc_VectorMagnitudeCalc(intersect->offsetX, Q12(0.0f), intersect->offsetZ),
                                    intersect->intersectAlpha,
                                    Q12_SHIFT);
            result[1] = intersect->triggerHeight;
        }
    }

    void Collision_TriggerIntersectInit(s_CollisionTriggerIntersect* intersect,
                                        q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ)
    {
        intersect->startX         = posX;
        intersect->startZ         = posZ;
        intersect->offsetX        = offsetX;
        intersect->endX           = posX + offsetX;
        intersect->intersectAlpha = Q12(1.0f);
        intersect->triggerHeight  = DEFAULT_TRIGGER_HEIGHT;
        intersect->offsetZ        = offsetZ;
        intersect->endZ           = posZ + offsetZ;

        if (intersect->endX >= intersect->startX)
        {
            intersect->aabbMinX = intersect->startX;
            intersect->aabbMaxX = intersect->endX;
        }
        else
        {
            intersect->aabbMinX = posX + offsetX;
            intersect->aabbMaxX = intersect->startX;
        }

        if (intersect->endZ >= intersect->startZ)
        {
            intersect->aabbMinZ = intersect->startZ;
            intersect->aabbMaxZ = intersect->endZ;
            return;
        }
        else
        {
            intersect->aabbMinZ = intersect->endZ;
            intersect->aabbMaxZ = intersect->startZ;
        }
    }

    bool Collision_TriggerIntersectGet(s_CollisionTriggerIntersect* intersect, const s_CollisionTrigger* trigger)
    {
        // Define AABB bounds.
        q19_12 aabbMinX = Q12(trigger->positionX);
        q19_12 aabbMaxX = Q12(trigger->positionX + trigger->sizeX);
        q19_12 aabbMinZ = Q12(trigger->positionZ);
        q19_12 aabbMaxZ = Q12(trigger->positionZ + trigger->sizeZ);

        // Check for AABB intersection.
        if ((aabbMinX >= intersect->aabbMaxX || intersect->aabbMinX >= aabbMaxX) &&
            (aabbMinZ >= intersect->aabbMaxZ || intersect->aabbMinZ >= aabbMaxZ))
        {
            return false;
        }

        if (intersect->startX >= aabbMinX && aabbMaxX >= intersect->startX &&
            intersect->startZ >= aabbMinZ && aabbMaxZ >= intersect->startZ)
        {
            intersect->intersectAlpha = Q12(0.0f);
            intersect->triggerHeight  = TRIGGER_HEIGHT_GET(trigger->height);
        }
        else
        {
            q19_12 edgeX = Q12(0.0f);
            q19_12 edgeZ = Q12(0.0f);
            if (intersect->offsetX >= Q12(0.0f))
            {
                if (intersect->offsetZ >= Q12(0.0f))
                {
                    edgeX = aabbMinX;
                    edgeZ = aabbMinZ;
                }
                else
                {
                    edgeX = aabbMinX;
                    edgeZ = aabbMaxZ;
                }
            }
            else
            {
                if (intersect->offsetZ >= Q12(0.0f))
                {
                    edgeX = aabbMaxX;
                    edgeZ = aabbMinZ;
                }
                else
                {
                    edgeX = aabbMaxX;
                    edgeZ = aabbMaxZ;
                }
            }

            // TODO: What are these shifts? Q12 division (x / Q12(16.0f))?
            q19_12 intersectAlphaX = Vw_LineSegmentIntersectionCheck(Q12(intersect->offsetX) >> 16,
                                                                     Q12(intersect->offsetZ) >> 16, Q12(edgeZ - intersect->startZ) >> 16,
                                                                     Q12(aabbMinX - intersect->startX) >> 16, Q12(aabbMaxX - intersect->startX) >> 16);
            q19_12 intersectAlphaZ = Vw_LineSegmentIntersectionCheck(Q12(intersect->offsetZ) >> 16,
                                                                     Q12(intersect->offsetX) >> 16, Q12(edgeX - intersect->startX) >> 16,
                                                                     Q12(aabbMinZ - intersect->startZ) >> 16, Q12(aabbMaxZ - intersect->startZ) >> 16);

            q19_12 intersectAlpha = Q12(0.0f);
            if (intersectAlphaX <= intersectAlphaZ)
            {
                intersectAlpha = intersectAlphaX;
            }
            else
            {
                intersectAlpha = intersectAlphaZ;
            }

            // Set intersection if new trigger is closer.
            if (intersectAlpha < intersect->intersectAlpha)
            {
                intersect->intersectAlpha = intersectAlpha;
                intersect->triggerHeight  = TRIGGER_HEIGHT_GET(trigger->height);
            }
        }

        return intersect->intersectAlpha == Q12(0.0f);
    }

    q19_12 Collision_TriggerCeilingHeightGet(VECTOR3* moveOffset,
                                             const s_CollisionCylinder* cylinder, q19_12 cylinderRadius, q19_12 cylinderTopExtent)
    {
        q19_12 ceilHeight  = Q12(DEFAULT_CEILING_HEIGHT);
        q19_12 pushOffsetX = Q12(0.0f);
        q19_12 pushOffsetZ = Q12(0.0f);

        // Compute start parameters.
        q19_12 moveOffsetX     = moveOffset->vx;
        q19_12 moveOffsetZ     = moveOffset->vz;
        q19_12 cylinderTop     = cylinder->position.vy + cylinderTopExtent;
        q19_12 projCylinderTop = cylinderTop + moveOffset->vy;

        // Run through active triggers.
        for (int i = 0; i < g_ActiveCollisionTriggers.collisionTriggerCount; i++)
        {
            const auto* curTrigger = g_ActiveCollisionTriggers.collisionTriggers[i];

            // Check if trigger overhead is avoided.
            q19_12 triggerHeight = TRIGGER_HEIGHT_GET(curTrigger->height);
            if ((projCylinderTop - triggerHeight) >= Q12(0.0f))
            {
                continue;
            }

            // Check rough cylinder-trigger collision at projected position.
            q19_12 projOffsetToTriggerX = Q12(0.0f);
            q19_12 projOffsetToTriggerZ = Q12(0.0f);
            Collision_TriggerOffsetGet(&projOffsetToTriggerX, &projOffsetToTriggerZ,
                                       cylinder->position.vx + moveOffsetX, cylinder->position.vz + moveOffsetZ,
                                       curTrigger);
            if (MAX(ABS(projOffsetToTriggerX), ABS(projOffsetToTriggerZ)) >= cylinderRadius)
            {
                continue;
            }

            // Check accurate cylinder-trigger collision at projected position.
            q19_12 projDistToTrigger = Vc_VectorMagnitudeCalc(projOffsetToTriggerX, Q12(0.0f), projOffsetToTriggerZ);
            if (projDistToTrigger >= cylinderRadius)
            {
                continue;
            }

            // Handle horizontal edge rejection.
            q19_12 offsetToTriggerX = Q12(0.0f);
            q19_12 offsetToTriggerZ = Q12(0.0f);
            if (projDistToTrigger > Q12(0.0f))
            {
                Collision_TriggerOffsetGet(&offsetToTriggerX, &offsetToTriggerZ,
                                           cylinder->position.vx, cylinder->position.vz,
                                           curTrigger);

                // Compute allowed intersection depth.
                q19_12 allowedIntersectDist = INTERSECTION_BUFFER;
                q19_12 distToTrigger        = Vc_VectorMagnitudeCalc(offsetToTriggerX, Q12(0.0f), offsetToTriggerZ);
                if ((cylinderRadius - distToTrigger) <= INTERSECTION_BUFFER)
                {
                    allowedIntersectDist = cylinderRadius - distToTrigger;
                }

                // Check for projected intersection.
                if ((projDistToTrigger - distToTrigger) < allowedIntersectDist)
                {
                    // Compute push distance away from trigger.
                    q19_12 angleToTrigger = ratan2(projOffsetToTriggerX, projOffsetToTriggerZ);
                    q19_12 pushDist       = allowedIntersectDist - (projDistToTrigger - distToTrigger);

                    // Compute push offset.
                    pushOffsetX = Math_MulFixed(pushDist, Math_Sin(angleToTrigger), Q12_SHIFT);
                    pushOffsetZ = Math_MulFixed(pushDist, Math_Cos(angleToTrigger), Q12_SHIFT);
                }
            }
            // Handle predictive ceiling rejection.
            else
            {
                // Track lowest ceiling.
                if (triggerHeight < ceilHeight)
                {
                    ceilHeight = triggerHeight;
                }

                // Reset move offset.
                moveOffsetX = moveOffset->vx;
                moveOffsetZ = moveOffset->vz;
                break;
            }

            // Apply push offset.
            moveOffsetX += pushOffsetX;
            moveOffsetZ += pushOffsetZ;
        }

        // Update movement offset.
        moveOffset->vx = moveOffsetX;
        moveOffset->vz = moveOffsetZ;

        // Restrict vertical movement.
        if (ceilHeight != Q12(DEFAULT_CEILING_HEIGHT))
        {
            // Compute headroom.
            q19_12 headroom              = INTERSECTION_BUFFER;
            q19_12 cylinderTopToCeilDist = ceilHeight - cylinderTop;
            if (cylinderTopToCeilDist < INTERSECTION_BUFFER)
            {
                headroom = cylinderTopToCeilDist;
            }

            // Clamp movement to ceiling.
            if (moveOffset->vy < headroom)
            {
                moveOffset->vy = headroom;
            }
        }

        return ceilHeight;
    }

    void Collision_TriggerOffsetGet(q19_12* offsetX, q19_12* offsetZ, q19_12 posX, q19_12 posZ,
                                    const s_CollisionTrigger* trigger)
    {
        // Define AABB bounds.
        q19_12 minX = FP_TO(trigger->positionX, Q12_SHIFT);
        q19_12 maxX = FP_TO(trigger->positionX + trigger->sizeX, Q12_SHIFT);
        q19_12 minZ = FP_TO(trigger->positionZ, Q12_SHIFT);
        q19_12 maxZ = FP_TO(trigger->positionZ + trigger->sizeZ, Q12_SHIFT);

        if (posX < minX)
        {
            *offsetX = posX - minX;
        }
        else if (maxX >= posX)
        {
            *offsetX = Q12(0.0f);
        }
        else
        {
            *offsetX = posX - maxX;
        }

        if (posZ < minZ)
        {
            *offsetZ = posZ - minZ;
            return;
        }
        else if (maxZ >= posZ)
        {
            *offsetZ = Q12(0.0f);
            return;
        }
        else
        {
            *offsetZ = posZ - maxZ;
            return;
        }
    }
}

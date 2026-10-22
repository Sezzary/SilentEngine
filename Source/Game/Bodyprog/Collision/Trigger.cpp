#include "Framework.h"
#include "Game/Bodyprog/Collision/Trigger.h"

#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Collision/Utils.h"
#include "Game/Bodyprog/View/VcMain.cpp"

namespace Silent::Game
{
    /** @brief Computes a trigger height from half-meter height steps.
     *
     * @note The trigger height has a default offset of `Q12(-1.5f)`.
     *
     * @param steps Half-meter height steps.
     * @return Trigger height.
     */
    constexpr q19_12 TRIGGER_HEIGHT_GET(int steps)
    {
        return (-Q12(steps) >> 1) - Q12(1.5f);
    }

    // @todo Add these defs when deobfuscated in the decomp.
    //void func_8006F250(q19_12* arg0, q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ);
    //void func_8006F338(s_func_8006F338* arg0, q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ);
    //bool func_8006F3C4(s_func_8006F338* arg0, const s_CollisionTrigger* trigger);

    q19_12 Collision_CeilingHeightGet(VECTOR3* moveOffset,
                                      const s_CollisionCylinder* cylinder, q19_12 cylinderRadius, q19_12 cylinderHeight)
    {
        q19_12                    projOffsetToTriggerX;
        q19_12                    projOffsetToTriggerZ;
        q19_12                    offsetToTriggerX;
        q19_12                    offsetToTriggerZ;
        q19_12                    cylinderTopToCeilDist;
        q19_12                    distToTrigger;
        q19_12                    pushDist;
        q19_12                    angleToTrigger;
        q19_12                    allowedIntersectDist;
        q19_12                    headroom;

        q19_12 ceilHeight  = Q12(DEFAULT_CEILING_HEIGHT);
        q19_12 pushOffsetX = Q12(0.0f);
        q19_12 pushOffsetZ = Q12(0.0f);

        // Compute start parameters.
        q19_12 moveOffsetX     = moveOffset->vx;
        q19_12 moveOffsetZ     = moveOffset->vz;
        q19_12 cylinderTop     = cylinder->position.vy + cylinderHeight;
        q19_12 projCylinderTop = cylinderTop + moveOffset->vy;

        // Run through collision triggers.
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
            if (projDistToTrigger > Q12(0.0f))
            {
                Collision_TriggerOffsetGet(&offsetToTriggerX, &offsetToTriggerZ,
                                        cylinder->position.vx, cylinder->position.vz,
                                        curTrigger);

                // Compute allowed intersection depth.
                allowedIntersectDist = INTERSECTION_BUFFER;
                distToTrigger        = Vc_VectorMagnitudeCalc(offsetToTriggerX, Q12(0.0f), offsetToTriggerZ);
                if ((cylinderRadius - distToTrigger) <= INTERSECTION_BUFFER)
                {
                    allowedIntersectDist = cylinderRadius - distToTrigger;
                }

                // Check for projected intersection.
                if ((projDistToTrigger - distToTrigger) < allowedIntersectDist)
                {
                    // Compute push distance away from trigger.
                    angleToTrigger = ratan2(projOffsetToTriggerX, projOffsetToTriggerZ);
                    pushDist       = allowedIntersectDist - (projDistToTrigger - distToTrigger);

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
            headroom              = INTERSECTION_BUFFER;
            cylinderTopToCeilDist = ceilHeight - cylinderTop;
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
        q19_12 minX = Q12(trigger->positionX);
        q19_12 maxX = Q12(trigger->positionX + trigger->sizeX);
        q19_12 minZ = Q12(trigger->positionZ);
        q19_12 maxZ = Q12(trigger->positionZ + trigger->sizeZ);

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

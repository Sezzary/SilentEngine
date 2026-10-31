#pragma once

namespace Silent::Game
{
    struct s_CollisionCylinder;

    /** @brief Collision trigger flags. */
    enum e_CollisionTriggerFlags
    {
        CollisionTriggerFlag_None    = 0,
        CollisionTriggerFlag_Map     = 1 << 0,
        CollisionTriggerFlag_Objects = 1 << 1,
        CollisionTriggerFlag_Block   = 1 << 2, /** Blocks select passages, e.g. the gate to the Split Head boss elevator. */
        CollisionTriggerFlag_Unused3 = 1 << 3, /** @unused Secondary block? Only activated in `MAP6_S05` and never checked. */
        CollisionTriggerFlag_All     = 0xFFFF
    };

    /** @brief Collision trigger line intersection. */
    struct s_CollisionTriggerIntersect
    {
        q19_12 startX;         /** } Line start. */
        q19_12 startZ;         /** } */
        q19_12 endX;           /** } Line end (start + offset). */
        q19_12 endZ;           /** } */
        q19_12 offsetX;        /** } Offset from start to end. */
        q19_12 offsetZ;        /** } */
        q19_12 aabbMinX;       /** } AABB encompassing the line. */
        q19_12 aabbMaxX;       /** } */
        q19_12 aabbMinZ;       /** } */
        q19_12 aabbMaxZ;       /** } */
        q19_12 intersectAlpha; /** Intersection alpha along the line with a collided trigger. */
        q19_12 triggerHeight;  /** Collided trigger height. */
    };

    /** @brief World-space collision trigger used to inform uneven terrain such as walls, ceilings, and stairs. */
    struct s_CollisionTrigger
    {
        bool isEndOfArray; /** `bool` | Marks last entry. */
        int  positionX;    /** Meter steps. */
        int  positionZ;    /** Meter steps. */
        int  sizeX;        /** Meter steps. */
        int  sizeZ;        /** Meter steps. */
        int  height;       /** Half-meter steps. */
    };

    /** @brief Collection of nearby collision triggers. */
    struct s_ActiveCollisionTriggers
    {
        int                       flags;                  /** `e_CollisionTriggerFlags` */
        int                       collisionTriggerCount;  /** `collisionTrigger` array size. */
        const s_CollisionTrigger* collisionTriggers[256]; // @todo Store in `std::vector` and let it resize itself.
    };

    /** @brief Queries a line intersection with the first collided active collision trigger.
     *
     * @param result Output [0]: movement distance to the collided trigger and [1]: trigger height.
     * @param posX Start line position X.
     * @param posZ Start line position Z.
     * @param offsetX Line end offset from start position X.
     * @param offsetZ Line end offset from start position Z.
     */
    void Collision_TriggerQuery(q19_12* result, q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ);

    /** @brief Initializes a collision trigger line intersection.
     *
     * @param intersect Line intersection to initialize.
     * @param posX Start line position X.
     * @param posZ Start line position Z.
     * @param offsetX Line end offset from start X.
     * @param offsetZ Line end offset from start Z.
     */
    void Collision_TriggerIntersectInit(s_CollisionTriggerIntersect* intersect,
                                        q19_12 posX, q19_12 posZ, q19_12 offsetX, q19_12 offsetZ);

    /** @brief Checks a line-trigger intersection, storing the result in an output.
     *
     * @param intersect Output line intersection to check and update.
     * @param trigger Trigger to check for intersection.
     * @return `true` if the trigger was intersected, `false` otherwise.
     */
    bool Collision_TriggerIntersectGet(s_CollisionTriggerIntersect* intersect, const s_CollisionTrigger* trigger);

    /** @brief Gets the ceiling height, derived from active collision triggers.
     *
     * @param moveOffset Output movement offset.
     * @param cylinder Collision cylinder.
     * @param cylinderRadius Cylinder radius.
     * @param cylinderTopExtent Cylinder top extent.
     * @return Trigger ceiling height, or `DEFAULT_CEILING_HEIGHT` if no ceiling exists.
     */
    q19_12 Collision_TriggerCeilingHeightGet(VECTOR3* moveOffset,
                                             const s_CollisionCylinder* cylinder, q19_12 cylinderRadius, q19_12 cylinderTopExtent);

    /** @brief Gets the offset from a world position to the edge of a collision trigger.
     * If the position is inside the trigger, the offset is (0, 0).
     *
     * @param offsetX Output offset X.
     * @param offsetZ Output offset Z.
     * @param posX Reference position X.
     * @param posZ Reference position Z.
     * @param trigger Reference collision trigger.
     */
    void Collision_TriggerOffsetGet(q19_12* offsetX, q19_12* offsetZ, q19_12 posX, q19_12 posZ,
                                    const s_CollisionTrigger* trigger);
}

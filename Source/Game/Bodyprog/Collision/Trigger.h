#pragma once

namespace Silent::Game
{
    /** @brief Collision cylinder with a character collision state. TODO: Probably doesn't belong in this header. */
    struct s_CollisionCylinder
    {
        VECTOR3 position; /** Q19.12 */
        q3_12   bottom;
        q3_12   top;
        q3_12   radius;
        s8      collisionState; /** `e_CharaCollisionState` */
    };

    typedef struct
    {
        q19_12 field_0; // X position.
        q19_12 field_4; // Z position.
        q19_12 field_8;
        q19_12 field_C;
        q19_12 field_10; // X
        q19_12 field_14; // Z
        q19_12 field_18; // X } Bounds?
        q19_12 field_1C; // X }
        q19_12 field_20; // Z }
        q19_12 field_24; // Z }
        q19_12 field_28; // Maybe XZ position.
        q19_12 triggerHeight;
    } s_func_8006F338;

    /** @brief World-space collision trigger used to inform uneven terrain such as walls and stairs. */
    struct s_CollisionTrigger
    {
        bool isEndOfArray;   /** `bool` | Marks last entry. */
        int  positionX : 10; /** Meter steps. */
        int  positionZ : 10; /** Meter steps. */
        int  sizeX     : 4;  /** Meter steps. */
        int  sizeZ     : 4;  /** Meter steps. */
        int  height    : 3;  /** Half-meter steps. Used to set `s_func_8006F338::field_2C` which is then copied by `func_8006F250`. */
    };

    /** @brief Collection of nearby collision triggers. */
    struct s_ActiveCollisionTriggers
    {
        u16                 flags; /** `e_CollisionTriggerFlags` */
        u8                  collisionTriggerCount;
        s_CollisionTrigger* collisionTriggers[20]; // Guessed size.
    };

    void func_8006F250(q19_12* arg0, q19_12 posX, q19_12 posZ, q19_12 posDeltaX, q19_12 posDeltaZ);

    void func_8006F338(s_func_8006F338* arg0, q19_12 posX, q19_12 posZ, q19_12 posDeltaX, q19_12 posDeltaZ);

    bool func_8006F3C4(s_func_8006F338* arg0, const s_CollisionTrigger* trigger);

    /** @brief Gets the ceiling height, derived from active collision triggers.
     *
     * @param moveOffset Movement offset.
     * @param cylinder Collision cylinder.
     * @param cylinderRadius Cylinder radius.
     * @param cylinderHeight Cylinder height.
     * @return Ceiling height. `DEFAULT_CEILING_HEIGHT` if no ceiling exists.
     */
    q19_12 Collision_CeilingHeightGet(VECTOR3* moveOffset,
                                      const s_CollisionCylinder* cylinder, q19_12 cylinderRadius, q19_12 cylinderHeight);

    /** @brief Gets the offset from a world position to the edge of a collision trigger.
     * If the position is inside the trigger, the offset is (0, 0).
     *
     * @param offsetX Output offset X.
     * @param offsetZ Output offset Z.
     * @param posX World reference position X.
     * @param posZ World reference position Z.
     * @param trigger Reference collision trigger.
     */
    void Collision_TriggerOffsetGet(q19_12* offsetX, q19_12* offsetZ, q19_12 posX, q19_12 posZ,
                                    const s_CollisionTrigger* trigger);
}

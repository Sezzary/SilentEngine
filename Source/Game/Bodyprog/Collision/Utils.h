#pragma once

namespace Silent::Game
{
    struct s_CollisionTrigger;

    /** @brief Global collision flags.
     * Applies for both NPCs and the player.
     */
    enum e_CollisionTriggerFlags
    {
        CollisionTriggerFlag_None    = 0,
        CollisionTriggerFlag_Map     = 1 << 0,
        CollisionTriggerFlag_Objects = 1 << 1,
        CollisionTriggerFlag_2       = 1 << 2, // Enables alternative objects collisions?
        CollisionTriggerFlag_3       = 1 << 3, /** @unused Only ever called in `MAP6_S05`. */
        CollisionTriggerFlag_All     = 0xFFFF
    };

    extern s_ActiveCollisionTriggers g_ActiveCollisionTriggers;

    /** @brief Initializes the collision subsystem, resetting flags and clearing the trigger zone count. */
    void Collision_Init(void);

    /** @brief Gets the active collision flags.
     *
     * @return Active collision flags.
     */
    int Collision_FlagsGet(void);

    /** @brief Sets the active collision flags.
     *
     * @param collFlags New collision flags.
     */
    void Collision_FlagsSet(int collFlags);

    /** @brief Sets additional active collision flags.
     *
     * @param collFlags Additional collision flags to set.
     */
    void Collision_FlagBitsSet(int collFlags);

    /** @brief Clears active collision flags. `CollisionTriggerFlag_Map` is excepted.
     *
     * @param collFlags Collision flags to clear.
     */
    void Collision_FlagBitsClear(int collFlags);

    /** @brief Runs through collision triggers and collects those containing the given XZ position.
     *
     * @param posX Query X position.
     * @param posZ Query Z position.
     * @param triggers Array of collision triggers.
     */
    void Collision_NearbyTriggersGet(q19_12 posX, q19_12 posZ, const s_CollisionTrigger* triggers);
}

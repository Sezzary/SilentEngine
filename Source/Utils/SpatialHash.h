#pragma once

namespace std
{
    template <>
    struct hash<Vector3i>
    {
        size_t operator()(const Vector3i& vec) const
        {
            size_t seed = 0;
            seed ^= hash<int>()(vec.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hash<int>()(vec.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hash<int>()(vec.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}

namespace Silent::Utils
{
    /** @brief 3D spatial hash. */
    class SpatialHash
    {
    private:
        struct Cell
        {
            std::set<int>          ObjectIds = {};
            AxisAlignedBoundingBox Aabb      = AxisAlignedBoundingBox();

            Cell(const AxisAlignedBoundingBox& aabb);
        };

        // =======
        // Fields
        // =======

        std::unordered_map<Vector3i, Cell> _cells = {}; /** Key = position, value = cell. */

        float   _cellSize        = 0.0f;
        Vector3 _cellAabbExtents = Vector3::Zero;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs a `SpatialHash` with cells of a specified size.
         *
         * @param cellSize Cell width, height, and depth.
         */
        SpatialHash(float cellSize);

        // ========
        // Getters
        // ========

        /** @brief Gets the number of objects bounded in the hash.
         *
         * @return Object count.
         */
        uint GetSize() const;

        /** @brief Gets all bounded object IDs in the hash.
         *
         * @return All object IDs in the hash.
         */
        std::set<int> GetBoundedObjectIds() const;

        /** @brief Gets all object IDs of cells which collide with a position.
         *
         * @param pos Collision position.
         * @return Object IDs whose bounds collide with the position.
         */
        std::set<int> GetBoundedObjectIds(const Vector3& pos) const;

        /** @brief Gets all object IDs of cells which collide with a ray.
         *
         * @param ray Collision ray.
         * @param dist Ray distance.
         * @return Object IDs whose bounds collide with the ray.
         */
        std::set<int> GetBoundedObjectIds(const Ray& ray, float dist) const;

        /** @brief Gets all object IDs of cells which collide with an AABB.
         *
         * @param aabb Collision AABB.
         * @return Object IDs whose bounds collide with the AABB.
         */
        std::set<int> GetBoundedObjectIds(const AxisAlignedBoundingBox& aabb) const;

        /** @brief Gets all object IDs of cells which collide with an OBB.
         *
         * @param obb Collision OBB.
         * @return Object IDs whose bounds collide with the OBB.
         */
        std::set<int> GetBoundedObjectIds(const OrientedBoundingBox& obb) const;

        /** @brief Gets all object IDs of cells which collide with a sphere.
         *
         * @param sphere Collision sphere.
         * @return Object IDs whose bounds collide with the sphere.
         */
        std::set<int> GetBoundedObjectIds(const BoundingSphere& sphere) const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the hash contains no objects.
         *
         * @return `true` if empty, otherwise `false.
         */
        bool IsEmpty() const;

        // ==========
        // Utilities
        // ==========

        void Insert(int objectId, const AxisAlignedBoundingBox& aabb);
        void Insert(int objectId, const OrientedBoundingBox& obb);
        void Insert(int objectId, const BoundingSphere& sphere);
        void Move(int objectId, const AxisAlignedBoundingBox& aabb, const AxisAlignedBoundingBox& prevAabb);
        void Move(int objectId, const OrientedBoundingBox& obb, OrientedBoundingBox& prevObb);
        void Move(int objectId, const BoundingSphere& sphere, const BoundingSphere& prevSphere);
        void Remove(int objectId, const AxisAlignedBoundingBox& prevAabb);
        void Remove(int objectId, const OrientedBoundingBox& prevObb);
        void Remove(int objectId, const BoundingSphere& prevSphere);

        // ======
        // Debug
        // ======

        /** @brief Displays debug information in the debug GUI. */
        void Debug() const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the key to the cell which collides with the position.
         *
         * @param pos Collision position.
         * @return Cell key.
         */
        Vector3i GetCellKey(const Vector3& pos) const;

        /** @brief Gets all object IDs of cells which collide with a ray.
         *
         * @param ray Collision ray.
         * @param dist Ray distance.
         * @return Keys of cells wich collide with the ray.
         */
        std::vector<Vector3i> GetCellKeys(const Ray& ray, float dist) const;
        std::vector<Vector3i> GetCellKeys(const AxisAlignedBoundingBox& aabb) const;
        std::vector<Vector3i> GetCellKeys(const OrientedBoundingBox& obb) const;
        std::vector<Vector3i> GetCellKeys(const BoundingSphere& sphere) const;

        void Insert(int objectId, const std::vector<Vector3i>& keys);
        void Remove(int objectId, const std::vector<Vector3i>& keys);
    };
}

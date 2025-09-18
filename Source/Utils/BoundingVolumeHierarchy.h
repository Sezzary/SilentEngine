#pragma once

// References:
// https://github.com/erincatto/box2d/blob/28adacf82377d4113f2ed00586141463244b9d10/src/dynamic_tree.c
// https://www.gdcvault.com/play/1025909/Math-for-Game-Developers-Dynamic

// NOTE: `_leafIdMap` is a hash map for convenience. If performance suffers with too many `Move` and `Remove` calls, a method with faster access can be implemented.
// However, it requires maintaining an odd index variable outside the BVH instance, so a hash map is preferred for the benefit of cleaner code.

namespace Silent::Utils
{
    /** @brief Bounding volume hierarchy build strategies. */
    enum class BvhBuildStrategy
    {
        Fast,     /** O(n): Fast build, okay quality. Top-down approach with median split. */
        Balanced, /** O(n * m): Efficient build, good quality. Top-down approach with constrained surface area heuristic. */
        Accurate  /** O(n²): Slow build, optimal quality. Top-down approach with exhaustive surface area heuristic. */
    };

    /** @brief Dynamic bounding volume hierarchy. */
    class BoundingVolumeHierarchy
    {
    private:
        struct Node
        {
            int                    ObjectId = NO_VALUE;                 /** Only stored by a leaf node. */
            AxisAlignedBoundingBox Aabb     = AxisAlignedBoundingBox(); /** AABB encompassing the object. */

            int Height       = 0;        /** Height of the node in the tree. */
            int ParentId     = NO_VALUE; /** Parent node ID. */
            int LeftChildId  = NO_VALUE; /** Left child node ID. */
            int RightChildId = NO_VALUE; /** Right child node ID. */

            bool IsLeaf() const;
        };

        // =======
        // Fields
        // =======

        std::vector<Node>            _nodes       = {};       /** Nested nodes. */
        std::stack<int>              _freeNodeIds = {};       /** Node IDs available for reuse. */
        std::unordered_map<int, int> _leafIdMap   = {};       /** Key = object ID, value = leaf ID. */
        int                          _rootId      = NO_VALUE; /** Root node ID. */

    public:
        // =============
        // Constructors
        // =============

        BoundingVolumeHierarchy() = default;
        BoundingVolumeHierarchy(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, BvhBuildStrategy strategy = BvhBuildStrategy::Balanced);

        // ========
        // Getters
        // ========

        /** @brief Gets the number of objects in the tree.
         *
         * @return Number of objects in the tree.
         */
        uint GetSize() const;

        /** @brief Gets all object IDs in the tree.
         *
         * @return `std::vector` containing all the object IDs in the tree.
         */
        std::vector<int> GetBoundedObjectIds() const;

        /** @brief Gets all object IDs of nodes which collide with an input ray.
         *
         * @param ray Collision ray.
         * @param dist Ray distance.
         * @return `std::vector` containing object IDs which collide with the ray.
         */
        std::vector<int> GetBoundedObjectIds(const Ray& ray, float dist) const;

        /** @brief Gets all object IDs of nodes which collide with an input sphere.
         *
         * @param sphere Collision sphere.
         * @return `std::vector` containing object IDs which collide with the sphere.
         */
        std::vector<int> GetBoundedObjectIds(const BoundingSphere& sphere) const;

        /** @brief Gets all object IDs of nodes which collide with an input AABB.
         *
         * @param aabb Collision AABB.
         * @return `std::vector` containing object IDs which collide with the AABB.
         */
        std::vector<int> GetBoundedObjectIds(const AxisAlignedBoundingBox& aabb) const;

        /** @brief Gets all object IDs of nodes which collide with an input OBB.
         *
         * @param obb Collision OBB.
         * @return `std::vector` containing object IDs which collide with the OBB.
         */
        std::vector<int> GetBoundedObjectIds(const OrientedBoundingBox& obb) const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the tree has no objects.
         *
         * @return `true` if the tree is empty, `false` otherwise.
         */
        bool IsEmpty() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Inserts a new object into the tree.
         *
         * @param objectId New arbitrary object ID of a new object.
         * @param aabb AABB encompassing the object.
         * @param boundary Extension boundary for the AABB.
         */
        void Insert(int objectId, const AxisAlignedBoundingBox& aabb, float boundary = 0.0f);

        /** @brief Moves an existing object in the tree.
         *
         * @param objectId Existing arbitrary object ID.
         * @param aabb AABB encompassing the object.
         * @param boundary Extension boundary for the AABB.
         */
        void Move(int objectId, const AxisAlignedBoundingBox& aabb, float boundary = 0.0f);

        /** @brief Removes an object from the tree.
         *
         * @param objectId Arbitrary object ID to remove.
         */
        void Remove(int objectId);

    private:
        // ==================
        // Collision helpers
        // ==================

        /** @brief Gets all object IDs of nodes which pass an input collision test routine.
         *
         * @param testCollRoutine Collision test routine.
         * @return `std::vector` containing object IDs which pass the collision test routine.
         */
        std::vector<int> GetBoundedObjectIds(const std::function<bool(const Node& node)>& testCollRoutine) const;

        // ================
        // Dynamic helpers
        // ================

        /** @brief Allocates memory for a new node and gets its ID.
         *
         * @return New node ID.
         */
        int GetNewNodeId();
        
        /** @brief Uses the branch and bound method to get the best sibling leaf node ID.
         *
         * @param leafId Leaf node ID for which to get the best sibling.
         * @return Best sibling leaf ID.
         */
        int GetBestSiblingLeafId(int leafId) const;

        /** @brief Inserts a new leaf node into the tree.
         *
         * @param leafId New leaf node ID generate by `GetNewNodeId`.
         */
        void InsertLeaf(int leafId);

        /** @brief Removes an existing leaf node from the tree, pruning up the branch if needed.
         *
         * @param leafId Leaf node ID to remove.
         */
        void RemoveLeaf(int leafId);

        /** @brief Refits a given node's AABB, retreading its branch to automatically rebalance via `BalanceNode`.
         *
         * @param nodeId Node ID to refit.
         */
        void RefitNode(int nodeId);

        /** @brief Removes a node from the tree, gracefully managing memory.
         *
         * @param nodeId Node ID to remove.
         */
        void RemoveNode(int nodeId);

        /** @brief Performs a left or right tree rotation if the input node is imbalanced and returns new subtree root ID.
         *
         * @param nodeId Node ID to balance.
         * @return New subtree root ID.
         */
        int BalanceNode(int nodeId);

        // ===============
        // Static helpers
        // ===============

        /** @brief Builds an optimally balanced tree.
         *
         * @param objectIds Arbitrary object IDs to insert into the tree.
         * @param aabbs AABBs encompassing the objects.
         * @param strategy Tree build strategy.
         */
        void Build(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, BvhBuildStrategy strategy);

        /** @brief Recursively builds an optimally balanced tree. Called by the other `Build` overload.
         *
         * @param objectIds Arbitrary object IDs to insert into the tree.
         * @param aabbs AABBs encompassing the objects.
         * @param start Start range.
         * @param end End range.
         * @param strategy Tree build strategy.
         * @return New inner node.
         */
        int Build(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, int start, int end, BvhBuildStrategy strategy);

        // ==============
        // Debug helpers
        // ==============

        /** @brief Validates the correctness of the tree.
         * Used for debugging.
         */
        void Validate() const;

        /** @brief Recursively validates the correctness of the tree from the given node. Called by the other `Validate` overload.
         * Used for debugging.
         *
         * @param nodeId Node ID to validate.
         */
        void Validate(int nodeId) const;
    };
}

#include "Framework.h"
#include "Utils/BoundingVolumeHierarchy.h"

namespace Silent::Utils
{
    bool BoundingVolumeHierarchy::Node::IsLeaf() const
    {
        return LeftChildId == NO_VALUE && RightChildId == NO_VALUE;
    }

    BoundingVolumeHierarchy::BoundingVolumeHierarchy(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, BvhBuildStrategy strategy)
    {
        Debug::Assert(objectIds.size() == aabbs.size(), "BVH object ID and AABB counts unequal in static constructor.");

        if (objectIds.empty() && aabbs.empty())
        {
            return;
        }

        Build(objectIds, aabbs, strategy);
    }

    uint BoundingVolumeHierarchy::GetSize() const
    {
        return (uint)_leafIdMap.size();
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds() const
    {
        auto objectIds = std::vector<int>{};
        if (_leafIdMap.empty())
        {
            return objectIds;
        }

        // Collect all object IDs.
        objectIds.reserve(_leafIdMap.size());
        for (const auto& [keyObjectId, leafId] : _leafIdMap)
        {
            objectIds.push_back(keyObjectId);
        }

        return objectIds;
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds(const Ray& ray, float dist) const
    {
        auto testColl = [&](const Node& node)
        {
            auto intersectDist = ray.Intersects(node.Aabb);
            return intersectDist.has_value() ? (*intersectDist <= dist) : false;
        };

        return GetBoundedObjectIds(testColl);
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds(const AxisAlignedBoundingBox& aabb) const
    {
        auto testColl = [&](const Node& node)
        {
            return node.Aabb.Intersects(aabb);
        };

        return GetBoundedObjectIds(testColl);
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds(const OrientedBoundingBox& obb) const
    {
        auto testColl = [&](const Node& node)
        {
            return node.Aabb.Intersects(obb);
        };

        return GetBoundedObjectIds(testColl);
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds(const BoundingSphere& sphere) const
    {
        auto testColl = [&](const Node& node)
        {
            return node.Aabb.Intersects(sphere);
        };

        return GetBoundedObjectIds(testColl);
    }

    bool BoundingVolumeHierarchy::IsEmpty() const
    {
        return _leafIdMap.empty();
    }

    void BoundingVolumeHierarchy::Insert(int objectId, const AxisAlignedBoundingBox& aabb, float boundary)
    {
        // Find leaf containing object ID.
        auto it = _leafIdMap.find(objectId);
        if (it != _leafIdMap.end())
        {
            Debug::Log(fmt::format("BVH attempted to insert leaf with existing object ID {}.", objectId), Debug::LogLevel::Warning, Debug::LogMode::Debug, true);
            return;
        }

        // Allocate new leaf.
        int   leafId = GetNewNodeId();
        auto& leaf   = _nodes[leafId];

        // Set initial parameters.
        leaf.ObjectId = objectId;
        leaf.Aabb     = AxisAlignedBoundingBox(aabb.Center, aabb.Extents + Vector3(boundary));
        leaf.Height   = 0;

        // Insert new leaf.
        InsertLeaf(leafId);
    }

    void BoundingVolumeHierarchy::Move(int objectId, const AxisAlignedBoundingBox& aabb, float boundary)
    {
        // Find leaf containing object ID.
        auto it = _leafIdMap.find(objectId);
        if (it == _leafIdMap.end())
        {
            Debug::Log(fmt::format("BVH attempted to move missing leaf with object ID {}.", objectId), Debug::LogLevel::Warning, Debug::LogMode::Debug, true);
            return;
        }

        // Get leaf.
        const auto& [keyObjectId, leafId] = *it;
        auto&       leaf                  = _nodes[leafId];

        // Test if object AABB is inside node AABB.
        if (leaf.Aabb.Contains(aabb) == ContainmentType::Contains)
        {
            auto  deltaExtents = leaf.Aabb.Extents - aabb.Extents;
            float threshold    = boundary * 2.0f;

            // Test if object AABB is significantly smaller than node AABB.
            if (deltaExtents.x < threshold &&
                deltaExtents.y < threshold &&
                deltaExtents.z < threshold)
            {
                return;
            }
        }

        // Reinsert leaf.
        RemoveLeaf(leafId);
        Insert(objectId, aabb, boundary);
    }

    void BoundingVolumeHierarchy::Remove(int objectId)
    {
        // Find leaf containing object ID.
        auto it = _leafIdMap.find(objectId);
        if (it == _leafIdMap.end())
        {
            Debug::Log(fmt::format("BVH attempted to remove missing leaf with object ID {}.", objectId), Debug::LogLevel::Warning, Debug::LogMode::Debug, true);
            return;
        }

        // Remove leaf.
        const auto& [keyObjectId, leafId] = *it;
        RemoveLeaf(leafId);
    }

    std::vector<int> BoundingVolumeHierarchy::GetBoundedObjectIds(const std::function<bool(const Node& node)>& testCollRoutine) const
    {
        auto objectIds = std::vector<int>{};
        if (_nodes.empty())
        {
            return objectIds;
        }

        // Traverse tree.
        auto nodeIds = std::stack<int>{};
        nodeIds.push(_rootId);
        while (!nodeIds.empty())
        {
            int nodeId = nodeIds.top();
            nodeIds.pop();

            // Invalid node; continue.
            if (nodeId == NO_VALUE)
            {
                continue;
            }

            const auto& node = _nodes[nodeId];

            // Test node collision.
            if (!testCollRoutine(node))
            {
                continue;
            }

            // Leaf node; collect object ID.
            if (node.IsLeaf())
            {
                objectIds.push_back(node.ObjectId);
            }
            // Inner node; push children onto stack for traversal.
            else
            {
                if (node.LeftChildId != NO_VALUE)
                {
                    nodeIds.push(node.LeftChildId);
                }

                if (node.RightChildId != NO_VALUE)
                {
                    nodeIds.push(node.RightChildId);
                }
            }
        }

        return objectIds;
    }

    int BoundingVolumeHierarchy::GetNewNodeId()
    {
        int nodeId = 0;

        // Allocate and get new empty node ID.
        if (_freeNodeIds.empty())
        {
            _nodes.emplace_back();
            nodeId = (int)_nodes.size() - 1;
        }
        // Get existing empty node ID.
        else
        {
            nodeId = _freeNodeIds.top();
            _freeNodeIds.pop();
        }

        return nodeId;
    }

    int BoundingVolumeHierarchy::GetBestSiblingLeafId(int leafId) const
    {
        const auto& leaf = _nodes[leafId];

        // Branch and bound for best sibling leaf.
        int siblingId = _rootId;
        while (!_nodes[siblingId].IsLeaf())
        {
            const auto& sibling      = _nodes[siblingId];
            int         leftChildId  = sibling.LeftChildId;
            int         rightChildId = sibling.RightChildId;

            float inheritCost = leaf.Aabb.GetSurfaceArea() * 2.0f;

            // Calculate cost of creating new parent for sibling and new leaf.
            auto  mergedAabb = AxisAlignedBoundingBox::Merge(sibling.Aabb, leaf.Aabb);
            float mergedArea = mergedAabb.GetSurfaceArea();
            float cost       = mergedArea * 2.0f;

            // Calculate cost of descending into left child.
            float leftCost = FLT_MAX;
            if (leftChildId != NO_VALUE)
            {
                const auto& leftChild = _nodes[leftChildId];
                auto        aabb      = AxisAlignedBoundingBox::Merge(leftChild.Aabb, leaf.Aabb);
                float       newArea   = aabb.GetSurfaceArea();

                leftCost = leftChild.IsLeaf() ? (newArea + inheritCost) : ((newArea - leftChild.Aabb.GetSurfaceArea()) + inheritCost);
            }

            // Calculate cost of descending into right child.
            float rightCost = FLT_MAX;
            if (rightChildId != NO_VALUE)
            {
                const auto& rightChild = _nodes[rightChildId];
                auto        aabb       = AxisAlignedBoundingBox::Merge(rightChild.Aabb, leaf.Aabb);
                float       newArea    = aabb.GetSurfaceArea();

                rightCost = rightChild.IsLeaf() ? (newArea + inheritCost) : ((newArea - rightChild.Aabb.GetSurfaceArea()) + inheritCost);
            }

            // Test if descent is worthwhile according to minimum cost.
            if (cost < leftCost && cost < rightCost)
            {
                break;
            }

            // Descend.
            siblingId = (leftCost < rightCost) ? leftChildId : rightChildId;
            if (siblingId == NO_VALUE)
            {
                Debug::Log("BVH sibling leaf search failed.", Debug::LogLevel::Warning);
                break;
            }
        }

        return siblingId;
    }

    void BoundingVolumeHierarchy::InsertLeaf(int leafId)
    {
        // Create root if empty.
        if (_rootId == NO_VALUE)
        {
            auto& leaf = _nodes[leafId];

            _leafIdMap.insert({ leaf.ObjectId, leafId });
            _rootId = leafId;
            return;
        }

        // Allocate new parent.
        int parentId = GetNewNodeId();
        auto& parent = _nodes[parentId];

        // Get sibling leaf and new leaf.
        int siblingId = GetBestSiblingLeafId(leafId);
        auto& sibling = _nodes[siblingId];
        auto& leaf    = _nodes[leafId];

        // Calculate merged AABB of sibling leaf and new leaf.
        auto aabb = AxisAlignedBoundingBox::Merge(sibling.Aabb, leaf.Aabb);

        // Get previous parent.
        int prevParentId = sibling.ParentId;

        // Update nodes.
        parent.Aabb         = aabb;
        parent.Height       = sibling.Height + 1;
        parent.ParentId     = prevParentId;
        parent.LeftChildId  = siblingId;
        parent.RightChildId = leafId;
        sibling.ParentId    = parentId;
        leaf.ParentId       = parentId;

        if (prevParentId == NO_VALUE)
        {
            _rootId = parentId;
        }
        else
        {
            // Update previous parent's child reference.
            auto& prevParent = _nodes[prevParentId];
            if (prevParent.LeftChildId == siblingId)
            {
                prevParent.LeftChildId = parentId;
            }
            else
            {
                prevParent.RightChildId = parentId;
            }
        }

        // Refit.
        RefitNode(leafId);

        // Store object-leaf association.
        _leafIdMap.insert({ leaf.ObjectId, leafId });

        //Validate(leafId);
    }

    void BoundingVolumeHierarchy::RemoveLeaf(int leafId)
    {
        int nodeId   = leafId;
        int parentId = _nodes[nodeId].ParentId;

        // Remove node.
        RemoveNode(nodeId);

        // Prune branch up to root.
        while (parentId != NO_VALUE)
        {
            auto& parent = _nodes[parentId];

            // Check if parent becomes new leaf.
            int   siblingId = (parent.LeftChildId == nodeId) ? parent.RightChildId : parent.LeftChildId;
            auto& sibling   = _nodes[siblingId];

            // Rearrange nodes local to removal.
            if (parent.LeftChildId == nodeId || parent.RightChildId == nodeId)
            {
                // Replace parent with sibling.
                if (parent.ParentId != NO_VALUE)
                {
                    auto& grandparent = _nodes[parent.ParentId];
                    if (grandparent.LeftChildId == parentId)
                    {
                        grandparent.LeftChildId = siblingId;
                    }
                    else
                    {
                        grandparent.RightChildId = siblingId;
                    }

                    sibling.ParentId = parent.ParentId;
                }
                else
                {
                    // Sibling becomes root if no grandparent exists.
                    _rootId          = siblingId;
                    sibling.ParentId = NO_VALUE;
                }

                // Refit sibling (new parent).
                RefitNode(siblingId);

                // Remove previous parent.
                RemoveNode(parentId);
                parentId = sibling.ParentId;
            }
            // Refit up hierarchy.
            else
            {
                // Refit parent.
                RefitNode(parentId);
                parentId = NO_VALUE;
            }
        }
    }

    void BoundingVolumeHierarchy::RefitNode(int nodeId)
    {
        const auto& node = _nodes[nodeId];

        // Retread tree branch to refit AABBs.
        int parentId = node.ParentId;
        while (parentId != NO_VALUE)
        {
            // Balance node and get new subtree root.
            int   newParentId = BalanceNode(parentId);
            auto& parent      = _nodes[newParentId];

            if (parent.LeftChildId != NO_VALUE && parent.RightChildId != NO_VALUE)
            {
                const auto& leftChild  = _nodes[parent.LeftChildId];
                const auto& rightChild = _nodes[parent.RightChildId];

                parent.Aabb   = AxisAlignedBoundingBox::Merge(leftChild.Aabb, rightChild.Aabb);
                parent.Height = std::max(leftChild.Height, rightChild.Height) + 1;
            }
            else if (parent.LeftChildId != NO_VALUE)
            {
                const auto& leftChild = _nodes[parent.LeftChildId];

                parent.Aabb   = leftChild.Aabb;
                parent.Height = leftChild.Height + 1;
            }
            else if (parent.RightChildId != NO_VALUE)
            {
                const auto& rightChild = _nodes[parent.RightChildId];

                parent.Aabb   = rightChild.Aabb;
                parent.Height = rightChild.Height + 1;
            }

            parentId = parent.ParentId;
        }
    }

    void BoundingVolumeHierarchy::RemoveNode(int nodeId)
    {
        auto& node = _nodes[nodeId];

        // Remove leaf from map.
        if (node.IsLeaf())
        {
            _leafIdMap.erase(node.ObjectId);
        }

        // Clear node and mark free.
        node = {};
        _freeNodeIds.push(nodeId);

        // Shrink capacity if empty to avoid memory bloat.
        if (_nodes.size() == _freeNodeIds.size())
        {
            *this = {};
        }
    }

    int BoundingVolumeHierarchy::BalanceNode(int nodeId)
    {
        if (nodeId == NO_VALUE)
        {
            return nodeId;
        }

        auto& nodeA = _nodes[nodeId];
        if (nodeA.IsLeaf() || nodeA.Height < 2)
        {
            return nodeId;
        }

        int nodeIdB = nodeA.LeftChildId;
        int nodeIdC = nodeA.RightChildId;
        if (nodeIdB == NO_VALUE || nodeIdC == NO_VALUE)
        {
            return nodeId;
        }

        auto& nodeB = _nodes[nodeIdB];
        auto& nodeC = _nodes[nodeIdC];

        // Calculate balance.
        int balance = nodeC.Height - nodeB.Height;

        // Rotate C up.
        if (balance > 1)
        {
            int nodeIdF = nodeC.LeftChildId;
            int nodeIdG = nodeC.RightChildId;
            if (nodeIdF == NO_VALUE || nodeIdG == NO_VALUE)
            {
                return nodeId;
            }

            auto& nodeF = _nodes[nodeIdF];
            auto& nodeG = _nodes[nodeIdG];

            // Swap A and C.
            nodeC.ParentId    = nodeA.ParentId;
            nodeC.LeftChildId = nodeId;
            nodeA.ParentId    = nodeIdC;

            // Make A's previous parent point to C.
            if (nodeC.ParentId != NO_VALUE)
            {
                auto& parent = _nodes[nodeC.ParentId];
                if (parent.LeftChildId == nodeId)
                {
                    parent.LeftChildId = nodeIdC;
                }
                else
                {
                    parent.RightChildId = nodeIdC;
                }
            }
            else
            {
                _rootId = nodeIdC;
            }

            // Rotate.
            if (nodeF.Height > nodeG.Height)
            {
                nodeA.Aabb   = AxisAlignedBoundingBox::Merge(nodeB.Aabb, nodeG.Aabb);
                nodeC.Aabb   = AxisAlignedBoundingBox::Merge(nodeA.Aabb, nodeF.Aabb);
                nodeA.Height = std::max(nodeB.Height, nodeG.Height) + 1;
                nodeC.Height = std::max(nodeA.Height, nodeF.Height) + 1;

                nodeG.ParentId     = nodeId;
                nodeC.RightChildId = nodeIdF;
                nodeA.RightChildId = nodeIdG;
            }
            else
            {
                nodeA.Aabb   = AxisAlignedBoundingBox::Merge(nodeB.Aabb, nodeF.Aabb);
                nodeC.Aabb   = AxisAlignedBoundingBox::Merge(nodeA.Aabb, nodeG.Aabb);
                nodeA.Height = std::max(nodeB.Height, nodeF.Height) + 1;
                nodeC.Height = std::max(nodeA.Height, nodeG.Height) + 1;

                nodeF.ParentId     = nodeId;
                nodeC.RightChildId = nodeIdG;
                nodeA.RightChildId = nodeIdF;
            }

            return nodeIdC;
        }
        // Rotate B up.
        else if (balance < -1)
        {
            int nodeIdD = nodeB.LeftChildId;
            int nodeIdE = nodeB.RightChildId;
            if (nodeIdD == NO_VALUE || nodeIdE == NO_VALUE)
            {
                return nodeId;
            }

            auto& nodeD = _nodes[nodeIdD];
            auto& nodeE = _nodes[nodeIdE];

            // Swap A and B.
            nodeB.ParentId    = nodeA.ParentId;
            nodeB.LeftChildId = nodeId;
            nodeA.ParentId    = nodeIdB;

            // Make A's previous parent point to B.
            if (nodeB.ParentId != NO_VALUE)
            {
                auto& parent = _nodes[nodeB.ParentId];
                if (parent.LeftChildId == nodeId)
                {
                    parent.LeftChildId = nodeIdB;
                }
                else
                {
                    parent.RightChildId = nodeIdB;
                }
            }
            else
            {
                _rootId = nodeIdB;
            }

            // Rotate.
            if (nodeD.Height > nodeE.Height)
            {
                nodeA.Aabb   = AxisAlignedBoundingBox::Merge(nodeC.Aabb, nodeE.Aabb);
                nodeB.Aabb   = AxisAlignedBoundingBox::Merge(nodeA.Aabb, nodeD.Aabb);
                nodeA.Height = std::max(nodeC.Height, nodeE.Height) + 1;
                nodeB.Height = std::max(nodeA.Height, nodeD.Height) + 1;

                nodeB.RightChildId = nodeIdD;
                nodeA.LeftChildId  = nodeIdE;
                nodeE.ParentId     = nodeId;
            }
            else
            {
                nodeA.Aabb   = AxisAlignedBoundingBox::Merge(nodeC.Aabb, nodeD.Aabb);
                nodeB.Aabb   = AxisAlignedBoundingBox::Merge(nodeA.Aabb, nodeE.Aabb);
                nodeA.Height = std::max(nodeC.Height, nodeD.Height) + 1;
                nodeB.Height = std::max(nodeA.Height, nodeE.Height) + 1;

                nodeB.RightChildId = nodeIdE;
                nodeA.LeftChildId  = nodeIdD;
                nodeD.ParentId     = nodeId;
            }

            return nodeIdB;
        }

        return nodeId;
    }

    void BoundingVolumeHierarchy::Build(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, BvhBuildStrategy strategy)
    {
        // Reserve enough memory for optimally balanced tree.
        _nodes.reserve((objectIds.size() * 2) - 1);

        // Build tree recursively.
        Build(objectIds, aabbs, 0, (int)objectIds.size(), strategy);
        _rootId = (int)_nodes.size() - 1;

        //Validate();
    }

    int BoundingVolumeHierarchy::Build(const std::vector<int>& objectIds, const std::vector<AxisAlignedBoundingBox>& aabbs, int start, int end, BvhBuildStrategy strategy)
    {
        constexpr auto BALANCED_STRAT_SPLIT_RANGE_MAX = 10;

        // Range safety check.
        if (start >= end)
        {
            return NO_VALUE;
        }

        // Create new node.
        auto node = Node{};

        // Combine AABBs.
        node.Aabb = aabbs[start];
        for (int i = start + 1; i < end; i++)
        {
            node.Aabb = AxisAlignedBoundingBox::Merge(node.Aabb, aabbs[i]);
        }

        // Leaf node.
        if ((end - start) == 1)
        {
            int leafId = (int)_nodes.size();

            node.ObjectId = objectIds[start];
            node.Height   = 0;

            // Add new leaf.
            _nodes.push_back(node);
            _leafIdMap.insert({ node.ObjectId, leafId });
            return leafId;
        }
        // Inner node.
        else
        {
            auto getBestSplit = [&]()
            {
                int bestSplit = (start + end) / 2;

                // Fast strategy: median split.
                if (strategy == BvhBuildStrategy::Fast)
                {
                    return bestSplit;
                }

                float bestCost = FLT_MAX;
                int   range    = (strategy == BvhBuildStrategy::Balanced) ? BALANCED_STRAT_SPLIT_RANGE_MAX : (end - start);

                // Balanced or accurate strategy: surface area heuristic.
                for (int split = std::max(start + 1, bestSplit - range); split < std::min(end, bestSplit + range); split++)
                {
                    // Calculate AABB 0.
                    auto aabb0 = aabbs[start];
                    for (int i = start + 1; i < split; i++)
                    {
                        aabb0 = AxisAlignedBoundingBox::Merge(aabb0, aabbs[i]);
                    }

                    // Calculate AABB 1.
                    auto aabb1 = aabbs[split];
                    for (int i = split; i < end; i++)
                    {
                        aabb1 = AxisAlignedBoundingBox::Merge(aabb1, aabbs[i]);
                    }

                    // Calculate cost.
                    float area0 = aabb0.GetSurfaceArea();
                    float area1 = aabb1.GetSurfaceArea();
                    float cost  = (area0 * (split - start)) + (area1 * (end - split));

                    // Track best split.
                    if (cost < bestCost)
                    {
                        bestSplit = split;
                        bestCost  = cost;
                    }
                }

                return bestSplit;
            };

            int bestSplit = getBestSplit();

            // Create children recursively.
            node.LeftChildId  = Build(objectIds, aabbs, start,     bestSplit, strategy);
            node.RightChildId = Build(objectIds, aabbs, bestSplit, end,       strategy);

            // Set parent ID for children.
            int nodeId = (int)_nodes.size();
            if (node.LeftChildId != NO_VALUE)
            {
                _nodes[node.LeftChildId].ParentId = nodeId;
            }
            if (node.RightChildId != NO_VALUE)
            {
                _nodes[node.RightChildId].ParentId = nodeId;
            }

            // Set height.
            node.Height = std::max((node.LeftChildId  != NO_VALUE) ? _nodes[node.LeftChildId].Height  : 0, 
                                   (node.RightChildId != NO_VALUE) ? _nodes[node.RightChildId].Height : 0) + 1;

            // Add new inner node.
            _nodes.push_back(node);
            return nodeId;
        }
    }

    void BoundingVolumeHierarchy::Validate() const
    {
        if (Debug::IS_DEBUG_BUILD)
        {
            Validate(_rootId);

            // Validate unique object IDs.
            auto objectIds = GetBoundedObjectIds();
            for (int refObjectId : objectIds)
            {
                uint count = 0;
                for (int objectId : objectIds)
                {
                    if (refObjectId == objectId)
                    {
                        count++;
                    }
                }

                Debug::Assert(count == 1, "BVH duplicate object IDs contained.");
            }
        }
    }

    void BoundingVolumeHierarchy::Validate(int nodeId) const
    {
        if (Debug::IS_DEBUG_BUILD)
        {
            if (nodeId == NO_VALUE)
            {
                return;
            }

            // Get node.
            const auto& node = _nodes[nodeId];

            // Validate root.
            if (nodeId == _rootId)
            {
                Debug::Assert(node.ParentId == NO_VALUE, "BVH root node cannot have parent.");
            }

            // Validate leaf node.
            if (node.IsLeaf())
            {
                Debug::Assert(node.ObjectId != NO_VALUE, "BVH leaf node must contain object ID.");
                Debug::Assert(node.Height == 0, "BVH leaf node must have height of 0.");
            }
            // Validate inner node.
            else
            {
                Debug::Assert(node.ObjectId == NO_VALUE, "BVH inner node cannot contain object ID.");
                Debug::Assert(node.Height != 0, "BVH inner node cannot have height of 0.");
            }

            // Validate parent.
            if (nodeId != _rootId)
            {
                Debug::Assert(node.ParentId != NO_VALUE, "BVH non-root node must have parent.");
            }

            // Validate parent of children.
            if (node.LeftChildId != NO_VALUE)
            {
                const auto& leftChild = _nodes[node.LeftChildId];
                Debug::Assert(leftChild.ParentId == nodeId, "BVH left child has wrong parent.");
            }
            if (node.RightChildId != NO_VALUE)
            {
                const auto& rightChild = _nodes[node.RightChildId];
                Debug::Assert(rightChild.ParentId == nodeId, "BVH right child has wrong parent.");
            }

            // Validate height.
            if (nodeId != _rootId)
            {
                const auto& parent = _nodes[node.ParentId];
                Debug::Assert(node.Height < parent.Height, "BVH child height must be less than parent height.");
            }

            // Validate recursively.
            Validate(node.LeftChildId);
            Validate(node.RightChildId);
        }
    }
}

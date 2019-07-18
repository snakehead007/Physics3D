#pragma once

#include "../physical.h"
#include "buffers.h"

#include "../math/position.h"
#include "../math/fix.h"
#include "../math/bounds.h"

typedef Physical Boundable;

#define MAX_BRANCHES 4
#define LEAF_NODE_SIGNIFIER 0xFFFFFFFFFFFFFFFF

struct TreeNode {
private:
	void addToSubTrees(Boundable* obj, const Bounds& bounds);
public:
	Bounds bounds;
	union {
		TreeNode* subTrees;
		Boundable* object;
	};
	size_t nodeCount;

	inline bool isLeafNode() const { return nodeCount == LEAF_NODE_SIGNIFIER; }

	inline TreeNode() : nodeCount(LEAF_NODE_SIGNIFIER), object(nullptr) {};
	inline TreeNode(TreeNode* subTrees, size_t nodeCount) : subTrees(subTrees), nodeCount(nodeCount) {}
	inline TreeNode(Boundable* object, const Bounds& bounds) : nodeCount(LEAF_NODE_SIGNIFIER), object(object), bounds(bounds) {};

	inline TreeNode(const TreeNode&) = delete;
	inline void operator=(const TreeNode&) = delete;

	inline TreeNode(TreeNode&& other) : nodeCount(other.nodeCount), subTrees(other.subTrees), bounds(other.bounds) {
		other.subTrees = nullptr;
	}
	inline TreeNode& operator=(TreeNode&& other) {
		std::swap(this->nodeCount, other.nodeCount);
		std::swap(this->subTrees, other.subTrees);
		std::swap(this->bounds, other.bounds);
		return *this;
	}

	inline TreeNode* begin() const { return subTrees; }
	inline TreeNode* end() const { return subTrees+nodeCount; }

	inline ~TreeNode();

	void add(Boundable* obj, const Bounds& bounds);

	void recalculateBounds(bool strictBounds);
	void recalculateBoundsRecursive(bool strictBounds);

	void improveStructure();
};

struct BoundsTree {
	TreeNode rootNode;

	BoundsTree() : rootNode(new TreeNode[MAX_BRANCHES], 0) {

	}

	TreeNode* findLeafNodeFor(const Bounds& bounds) const {
		const TreeNode* currentNode = &rootNode;
		while (!currentNode->isLeafNode()) {

		}
	}

	void add(Boundable* obj, bool strictBounds) {
		rootNode.add(obj, strictBounds?obj->getStrictBounds():obj->getLooseBounds());
	}

	void update(bool strictBounds);
};
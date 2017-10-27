#pragma once

#include <map>
#include <stack>
#include <vector>

#include <tdme/engine/Transformations.h>
#include <tdme/engine/physics/World.h>
#include <tdme/engine/primitives/BoundingVolume.h>
#include <tdme/math/Math.h>
#include <tdme/math/Vector3.h>
#include <tdme/utils/PathFindingNode.h>
#include <tdme/utils/PathFindingCustomTest.h>

using std::map;
using std::stack;
using std::vector;

using tdme::engine::Transformations;
using tdme::engine::physics::World;
using tdme::engine::primitives::BoundingVolume;
using tdme::math::Math;
using tdme::math::Vector3;
using tdme::utils::PathFindingNode;
using tdme::utils::PathFindingCustomTest;

/**
 * Path Finding
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::utils::PathFinding final
{
public:
	enum PathFindingStatus {PATH_STEP, PATH_FOUND, PATH_NOWAY};

	/**
	 * Public constructor
	 * @param world
	 * @param user path finding test
	 * @param steps max
	 * @param step size
	 * @param step size last
	 * @param actor step up max
	 */
	PathFinding(World* world, PathFindingCustomTest* customTest = nullptr, int stepsMax = 1000, float stepSize = 0.5f, float stepSizeLast = 0.75f, float actorStepUpMax = 0.5f);

	/**
	 * Destructor
	 */
	~PathFinding();

	/**
	 * Format float for key usage
	 */
	string toKeyFloat(float value);

	/**
	 * Finds path to given end position
	 * @param actor original bounding volume
	 * @param current actor transformations
	 * @param end position
	 * @param collision rigidbody types
	 * @param path from actor to target
	 * @return success
	 */
	bool findPath(BoundingVolume* actorObv, Transformations* actorTransformations, const Vector3& endPosition, const uint32_t collisionRigidBodyTypes, vector<Vector3>& path);

private:
	/**
	 * Reset path finding
	 */
	void reset();

	/**
	 * Computes non square rooted distance between a and b
	 * @param a
	 * @param b
	 * @return non square rooted distance
	 */
	inline float computeDistance(PathFindingNode* a, PathFindingNode* b) {
		float dx = a->x - b->x;
		float dy = a->y - b->y;
		float dz = a->z - b->z;
		return (dx * dx) + (dy * dy) + (dz * dz);
	}

	/**
	 * Returns if nodes are equals
	 * @param a
	 * @param b
	 * @return if node a == node b
	 */
	inline bool equals(PathFindingNode* a, float bX, float bY, float bZ) {
		return
			(
				Math::abs(a->x - bX) < 0.1f &&
				Math::abs(a->y - bY) < 0.1f &&
				Math::abs(a->z - bZ) < 0.1f
			);
	}

	/**
	 * Returns if nodes are equals for (last node test)
	 * @param a
	 * @param b
	 * @return if node a == node b
	 */
	inline bool equalsLastNode(PathFindingNode* a, PathFindingNode* lastNode) {
		return
			(a == lastNode) ||
			(
				Math::abs(a->x - lastNode->x) < stepSizeLast &&
				Math::abs(a->y - lastNode->y) < stepSizeLast &&
				Math::abs(a->z - lastNode->z) < stepSizeLast
			);
	}

	/**
	 * Checks if a cell is walkable
	 * @param x
	 * @param y
	 * @param z
	 * @param y stepped up
	 * @return if cell is walkable
	 */
	bool isWalkable(float x, float y, float z, float& height);

	/**
	 * Sets up the PathFinding, it needs to be called after constructing the object
	 * @param start position
	 * @param end position
	 */
	void start(Vector3 startPosition, Vector3 endPosition);

	/**
	 * Processes one step in AStar path finding
	 * @return step status
	 */
	PathFindingStatus step();

	// properties
	World* world;
	PathFindingCustomTest* customTest;
	int stepsMax;
	float stepSize;
	float stepSizeLast;
	float actorStepUpMax;
	uint32_t collisionRigidBodyTypes;
	PathFindingNode* end;
	stack<PathFindingNode*> successorNodes;
	map<string, PathFindingNode*> openNodes;
	map<string, PathFindingNode*> closedNodes;
	Vector3 sideVector { 1.0f, 0.0f, 0.0f };
	Vector3 forwardVector { 0.0f, 0.0f, 1.0f };
	Transformations actorTransformations;
	BoundingVolume* actorObv;
	BoundingVolume* actorCbv;
};
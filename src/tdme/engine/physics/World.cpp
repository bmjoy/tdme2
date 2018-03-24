#include <tdme/engine/physics/World.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_set>
#include <string>

#include <ext/reactphysics3d/src/collision/ProxyShape.h>
#include <ext/reactphysics3d/src/collision/OverlapCallback.h>
#include <ext/reactphysics3d/src/collision/shapes/AABB.h>
#include <ext/reactphysics3d/src/engine/CollisionWorld.h>
#include <ext/reactphysics3d/src/engine/EventListener.h>
#include <ext/reactphysics3d/src/mathematics/Ray.h>
#include <ext/reactphysics3d/src/mathematics/Vector3.h>

#include <tdme/engine/Engine.h>
#include <tdme/engine/Entity.h>
#include <tdme/engine/Rotation.h>
#include <tdme/engine/Transformations.h>
#include <tdme/engine/physics/CollisionResponse.h>
#include <tdme/engine/physics/RigidBody.h>
#include <tdme/engine/primitives/BoundingBox.h>
#include <tdme/engine/primitives/BoundingVolume.h>
#include <tdme/engine/primitives/ConvexMeshBoundingVolume.h>
#include <tdme/engine/primitives/LineSegment.h>
#include <tdme/engine/primitives/OrientedBoundingBox.h>
#include <tdme/math/Math.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Quaternion.h>
#include <tdme/math/Vector3.h>
#include <tdme/utils/Console.h>
#include <tdme/utils/VectorIteratorMultiple.h>

using std::find;
using std::remove;
using std::map;
using std::string;
using std::to_string;
using std::unordered_set;

using tdme::engine::physics::World;
using tdme::engine::Engine;
using tdme::engine::Entity;
using tdme::engine::Rotation;
using tdme::engine::Transformations;
using tdme::engine::physics::CollisionResponse;
using tdme::engine::physics::RigidBody;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::primitives::ConvexMeshBoundingVolume;
using tdme::engine::primitives::BoundingVolume;
using tdme::engine::primitives::LineSegment;
using tdme::engine::primitives::OrientedBoundingBox;
using tdme::math::Math;
using tdme::math::Matrix4x4;
using tdme::math::Quaternion;
using tdme::math::Vector3;
using tdme::utils::Console;
using tdme::utils::VectorIteratorMultiple;

World::World(): world(reactphysics3d::Vector3(0.0, -9.81, 0.0))
{
}

World::~World()
{
	reset();
}

void World::reset()
{
	// TODO
}

RigidBody* World::addRigidBody(const string& id, bool enabled, uint16_t typeId, const Transformations& transformations, BoundingVolume* boundingVolume, float restitution, float friction, float mass, const Matrix4x4& inertiaMatrix)
{
	removeRigidBody(id);
	auto rigidBody = new RigidBody(this, id, RigidBody::TYPE_DYNAMIC, enabled, typeId, boundingVolume, transformations, restitution, friction, mass, inertiaMatrix);
	rigidBodies.push_back(rigidBody);
	rigidBodiesDynamic.push_back(rigidBody);
	rigidBodiesById[id] = rigidBody;
	return rigidBody;
}

RigidBody* World::addStaticRigidBody(const string& id, bool enabled, uint16_t typeId, const Transformations& transformations, BoundingVolume* boundingVolume, float friction)
{
	removeRigidBody(id);
	auto rigidBody = new RigidBody(this, id, RigidBody::TYPE_STATIC, enabled, typeId, boundingVolume, transformations, 0.0f, friction, 1000000.0f, RigidBody::computeInertiaMatrix(boundingVolume, 0.0f, 0.0f, 0.0f, 0.0f));
	rigidBodies.push_back(rigidBody);
	rigidBodiesById[id] = rigidBody;
	return rigidBody;
}

RigidBody* World::getRigidBody(const string& id)
{
	auto rididBodyByIdIt = rigidBodiesById.find(id);
	if (rididBodyByIdIt != rigidBodiesById.end()) {
		return rididBodyByIdIt->second;
	}
	return nullptr;
}

void World::removeRigidBody(const string& id) {
	auto rididBodyByIdIt = rigidBodiesById.find(id);
	if (rididBodyByIdIt != rigidBodiesById.end()) {
		auto rigidBody = rididBodyByIdIt->second;
		world.destroyRigidBody(rigidBody->rigidBody);
		rigidBodies.erase(remove(rigidBodies.begin(), rigidBodies.end(), rigidBody), rigidBodies.end());
		rigidBodiesDynamic.erase(remove(rigidBodiesDynamic.begin(), rigidBodiesDynamic.end(), rigidBody), rigidBodiesDynamic.end());
		rigidBodiesById.erase(rididBodyByIdIt);
		delete rigidBody;
	}
}

void World::update(float deltaTime)
{
	if (deltaTime < Math::EPSILON) return;

	// update velocities
	for (auto i = 0; i < rigidBodiesDynamic.size(); i++) {
		auto rigidBody = rigidBodiesDynamic.at(i);
		// skip if enabled
		if (rigidBody->enabled == false) {
			continue;
		}
		// skip if sleeping
		if (rigidBody->isSleeping() == true &&
			rigidBody->angularVelocity.computeLengthSquared() < Math::EPSILON &&
			rigidBody->linearVelocity.computeLengthSquared() < Math::EPSILON) {
			continue;
		}
		auto& rigidBodyAngularVelocity = rigidBody->angularVelocity;
		rigidBody->rigidBody->setAngularVelocity(
			reactphysics3d::Vector3(
				rigidBodyAngularVelocity.getX(),
				rigidBodyAngularVelocity.getY(),
				rigidBodyAngularVelocity.getZ()
			)
		);
		auto& rigidBodyLinearVelocity = rigidBody->linearVelocity;
		rigidBody->rigidBody->setLinearVelocity(
			reactphysics3d::Vector3(
				rigidBodyLinearVelocity.getX(),
				rigidBodyLinearVelocity.getY(),
				rigidBodyLinearVelocity.getZ()
			)
		);
	}

	// do the job
	world.update(deltaTime);

	// collision events
	{
		// fire on collision begin, on collision
		map<string, RigidBodyCollisionStruct> rigidBodyCollisionsCurrentFrame;
		CollisionResponse collision;
		auto manifolds = world.getContactsList();
		for (auto manifold: manifolds) {
			auto rigidBody1 = static_cast<RigidBody*>(manifold->getBody1()->getUserData());
			auto rigidBody2 = static_cast<RigidBody*>(manifold->getBody2()->getUserData());
			RigidBodyCollisionStruct rigidBodyCollisionStruct;
			rigidBodyCollisionStruct.rigidBody1Id = rigidBody1->getId();
			rigidBodyCollisionStruct.rigidBody2Id = rigidBody2->getId();
			string rigidBodyKey = rigidBodyCollisionStruct.rigidBody1Id + "," + rigidBodyCollisionStruct.rigidBody2Id;
			string rigidBodyKeyInverted = rigidBodyCollisionStruct.rigidBody2Id + "," + rigidBodyCollisionStruct.rigidBody1Id;
			rigidBodyCollisionsCurrentFrame[rigidBodyKey] = rigidBodyCollisionStruct;
			for (int i=0; i<manifold->getNbContactPoints(); i++) {
				auto contactPoint = manifold->getContactPoints();
				while (contactPoint != nullptr) {
					// construct collision
					auto entity = collision.addResponse(-contactPoint->getPenetrationDepth());
					auto normal = contactPoint->getNormal();
					entity->getNormal().set(normal.x, normal.y, normal.z);
					auto shape1 = manifold->getShape1();
					auto shape2 = manifold->getShape2();
					auto& shapeLocalToWorldTransform1 = shape1->getLocalToWorldTransform();
					auto& shapeLocalToWorldTransform2 = shape2->getLocalToWorldTransform();
					auto& localPoint1 = contactPoint->getLocalPointOnShape1();
					auto& localPoint2 = contactPoint->getLocalPointOnShape2();
					auto worldPoint1 = shapeLocalToWorldTransform1 * (localPoint1 * shape1->getLocalScaling());
					auto worldPoint2 = shapeLocalToWorldTransform2 * (localPoint2 * shape2->getLocalScaling());
					entity->addHitPoint(Vector3(worldPoint1.x, worldPoint1.y, worldPoint1.z));
					entity->addHitPoint(Vector3(worldPoint2.x, worldPoint2.y, worldPoint2.z));
					contactPoint = contactPoint->getNext();
					// fire events
					if (rigidBodyCollisionsLastFrame.find(rigidBodyKey) == rigidBodyCollisionsLastFrame.end() &&
						rigidBodyCollisionsLastFrame.find(rigidBodyKeyInverted) == rigidBodyCollisionsLastFrame.end()) {
						// fire on collision begin
						rigidBody1->fireOnCollisionBegin(rigidBody2, &collision);
					}
					// fire on collision
					rigidBody1->fireOnCollision(rigidBody2, &collision);
					// reset collision
					collision.reset();
				}
			}
		}

		// fire on collision end
		//	check each collision last frame that disappeared in current frame
		for (auto it: rigidBodyCollisionsLastFrame) {
			RigidBodyCollisionStruct* rigidBodyCollisionStruct = &it.second;
			{
				string rigidBodyKey = rigidBodyCollisionStruct->rigidBody1Id + "," + rigidBodyCollisionStruct->rigidBody2Id;
				auto rigidBodyCollisionsCurrentFrameIt = rigidBodyCollisionsCurrentFrame.find(rigidBodyKey);
				if (rigidBodyCollisionsCurrentFrameIt != rigidBodyCollisionsCurrentFrame.end()) continue;
			}
			{
				string rigidBodyKey = rigidBodyCollisionStruct->rigidBody2Id + "," + rigidBodyCollisionStruct->rigidBody1Id;
				auto rigidBodyCollisionsCurrentFrameIt = rigidBodyCollisionsCurrentFrame.find(rigidBodyKey);
				if (rigidBodyCollisionsCurrentFrameIt != rigidBodyCollisionsCurrentFrame.end()) continue;
			}
			auto rigidBody1It = rigidBodiesById.find(rigidBodyCollisionStruct->rigidBody1Id);
			auto rigidBody1 = rigidBody1It == rigidBodiesById.end()?nullptr:rigidBody1It->second;
			auto rigidBody2It = rigidBodiesById.find(rigidBodyCollisionStruct->rigidBody2Id);
			auto rigidBody2 = rigidBody2It == rigidBodiesById.end()?nullptr:rigidBody2It->second;
			if (rigidBody1 == nullptr || rigidBody2 == nullptr) continue;
			rigidBody1->fireOnCollisionEnd(rigidBody2);
		}
		// swap rigid body collisions current and last frame
		rigidBodyCollisionsLastFrame = rigidBodyCollisionsCurrentFrame;
	}

	// update transformations for rigid body
	for (auto i = 0; i < rigidBodiesDynamic.size(); i++) {
		auto rigidBody = rigidBodiesDynamic.at(i);
		// skip if enabled
		if (rigidBody->enabled == false) {
			continue;
		}
		// skip if static or sleeping
		if (rigidBody->isSleeping() == true) {
			continue;
		}

		auto& physicsTransformations = rigidBody->transformations;

		// set up transformations, keep care that only 1 rotation exists
		while (physicsTransformations.getRotationCount() > 1) {
			physicsTransformations.removeRotation(physicsTransformations.getRotationCount() - 1);
		}
		if (physicsTransformations.getRotationCount() < 1) {
			physicsTransformations.addRotation(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
		}
		// set up position, orientation
		auto transform = rigidBody->rigidBody->getTransform();
		auto& position = transform.getPosition();
		auto& orientation = transform.getOrientation();
		//	set up transformations
		physicsTransformations.setTranslation(Vector3(position.x, position.y, position.z));
		/*
		Console::println(
			"position: " + rigidBody->getId() + ": " +
			to_string(position.x) + ", " +
			to_string(position.y) + ", " +
			to_string(position.z)
		);
		physicsTransformations.getTranslation().sub(
			Vector3(
				rigidBody->boundingVolume->collisionShapeLocalTransform.getPosition().x,
				rigidBody->boundingVolume->collisionShapeLocalTransform.getPosition().y,
				rigidBody->boundingVolume->collisionShapeLocalTransform.getPosition().z
			)
		);
		*/
		physicsTransformations.setPivot(rigidBody->boundingVolume->getCenter());
		physicsTransformations.getRotation(0).fromQuaternion(Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
		physicsTransformations.update();
		// velocities
		auto angularVelocity = rigidBody->rigidBody->getAngularVelocity();
		auto linearVelocity = rigidBody->rigidBody->getLinearVelocity();
		rigidBody->getAngularVelocity().set(angularVelocity.x, angularVelocity.y, angularVelocity.z);
		rigidBody->getLinearVelocity().set(linearVelocity.x, linearVelocity.y, linearVelocity.z);
	}
}

void World::synch(Engine* engine)
{
	for (auto i = 0; i < rigidBodiesDynamic.size(); i++) {
		// update rigid body
		auto rigidBody = rigidBodiesDynamic.at(i);

		// skip on sleeping objects
		if (rigidBody->isSleeping() == true) continue;

		// synch with engine entity
		auto engineEntity = engine->getEntity(rigidBody->id);
		if (engineEntity == nullptr) {
			Console::println(
				string("World::entity '") +
				rigidBody->id +
				string("' not found")
			);
			continue;
		}

		// enable
		engineEntity->setEnabled(rigidBody->enabled);

		//apply inverse local transformation for engine update
		if (rigidBody->enabled == true) {
			engineEntity->fromTransformations(rigidBody->transformations);
		}
	}
}

RigidBody* World::determineHeight(int32_t typeIds, float stepUpMax, const Vector3& point, Vector3& dest)
{
	class CustomCallbackClass : public reactphysics3d::RaycastCallback {
	public:
		CustomCallbackClass(float stepUpMax, const Vector3& point): stepUpMax(stepUpMax), point(point), height(-10000.0f), rigidBody(nullptr) {
		}
		virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info) {
			Vector3 hitPoint(info.worldPoint.x, info.worldPoint.y, info.worldPoint.z);
			if (hitPoint.getY() >= height && hitPoint.getY() <= point.getY() + Math::max(0.1f, stepUpMax)) {
				height = hitPoint.getY();
				rigidBody = (RigidBody*)info.body->getUserData();
			}
			return reactphysics3d::decimal(1.0);
		};
		RigidBody* getRigidBody() {
			return rigidBody;
		}
		const float getHeight() {
			return height;
		}
	private:
		float stepUpMax;
		Vector3 point;
		float height;
		RigidBody* rigidBody;
	};
	reactphysics3d::Vector3 startPoint(point.getX(), +10000.0f, point.getZ());
	reactphysics3d::Vector3 endPoint(point.getX(), -10000.0f, point.getZ());
	reactphysics3d::Ray ray(startPoint, endPoint);
	CustomCallbackClass customCallbackObject(stepUpMax, point);
	world.raycast(ray, &customCallbackObject, typeIds);
	if (customCallbackObject.getRigidBody() != nullptr) {
		dest.set(point);
		dest.setY(customCallbackObject.getHeight());
		return customCallbackObject.getRigidBody();
	} else {
		return nullptr;
	}
}

RigidBody* World::determineHeight(int32_t typeIds, float stepUpMax, BoundingVolume* boundingVolume, const Vector3& point, Vector3& dest)
{
	auto determinedHeight = -10000.0f;
	Vector3 heightPoint;
	Vector3 heightPointDest;
	auto width = boundingVolume->getBoundingBoxTransformed().getDimensions().getX();
	auto height = boundingVolume->getBoundingBoxTransformed().getDimensions().getY();
	auto depth = boundingVolume->getBoundingBoxTransformed().getDimensions().getZ();
	float heightPointDestY;
	RigidBody* heightRigidBody = nullptr;
	RigidBody* rigidBody = nullptr;
	// center, center
	heightPoint.set(boundingVolume->getBoundingBoxTransformed().getCenter());
	heightPoint.addY(-height / 2.0f);
	rigidBody = determineHeight(typeIds, stepUpMax, heightPoint, heightPointDest);
	if (rigidBody != nullptr) {
		heightPointDestY = heightPointDest.getY();
		if (heightPointDestY > determinedHeight) {
			heightRigidBody = rigidBody;
			determinedHeight = heightPointDestY;
		}
	}
	// left, top
	heightPoint.set(boundingVolume->getBoundingBoxTransformed().getCenter());
	heightPoint.addX(-width / 2.0f);
	heightPoint.addY(-height / 2.0f);
	heightPoint.addZ(-depth / 2.0f);
	rigidBody = determineHeight(typeIds, stepUpMax, heightPoint, heightPointDest);
	if (rigidBody != nullptr) {
		heightPointDestY = heightPointDest.getY();
		if (heightPointDestY > determinedHeight) {
			heightRigidBody = rigidBody;
			determinedHeight = heightPointDestY;
		}
	}
	// left, bottom
	heightPoint.set(boundingVolume->getBoundingBoxTransformed().getCenter());
	heightPoint.addX(-width / 2.0f);
	heightPoint.addY(-height / 2.0f);
	heightPoint.addZ(+depth / 2.0f);
	rigidBody = determineHeight(typeIds, stepUpMax, heightPoint, heightPointDest);
	if (rigidBody != nullptr) {
		heightPointDestY = heightPointDest.getY();
		if (heightPointDestY > determinedHeight) {
			heightRigidBody = rigidBody;
			determinedHeight = heightPointDestY;
		}
	}
	// right, top
	heightPoint.set(boundingVolume->getBoundingBoxTransformed().getCenter());
	heightPoint.addX(+width / 2.0f);
	heightPoint.addY(-height / 2.0f);
	heightPoint.addZ(-depth / 2.0f);
	rigidBody = determineHeight(typeIds, stepUpMax, heightPoint, heightPointDest);
	if (rigidBody != nullptr) {
		heightPointDestY = heightPointDest.getY();
		if (heightPointDestY > determinedHeight) {
			heightRigidBody = rigidBody;
			determinedHeight = heightPointDestY;
		}
	}
	// right, bottom
	heightPoint.set(boundingVolume->getBoundingBoxTransformed().getCenter());
	heightPoint.addX(+width / 2.0f);
	heightPoint.addY(-height / 2.0f);
	heightPoint.addZ(+depth / 2.0f);
	rigidBody = determineHeight(typeIds, stepUpMax, heightPoint, heightPointDest);
	if (rigidBody != nullptr) {
		heightPointDestY = heightPointDest.getY();
		if (heightPointDestY > determinedHeight) {
			heightRigidBody = rigidBody;
			determinedHeight = heightPointDestY;
		}
	}
	// set up result
	if (heightRigidBody == nullptr) {
		return nullptr;
	} else {
		dest.set(point);
		dest.setY(determinedHeight);
		return heightRigidBody;
	}
}

bool World::doesCollideWith(int32_t typeIds, BoundingBox* boundingBox, vector<RigidBody*>& rigidBodies) {
	// callback
	class CustomOverlapCallback: public reactphysics3d::OverlapCallback {
	    public:
			CustomOverlapCallback(vector<RigidBody*>& rigidBodies): rigidBodies(rigidBodies) {
			}

			virtual void notifyOverlap(reactphysics3d::CollisionBody* collisionBody) {
				rigidBodies.push_back(static_cast<RigidBody*>(collisionBody->getUserData()));
			}
	    private:
			vector<RigidBody*>& rigidBodies;
	};

	// create rp3d bounding box
	auto& bbMin = boundingBox->getMin();
	auto& bbMax = boundingBox->getMax();
	reactphysics3d::AABB aabb(
		reactphysics3d::Vector3(
			bbMin.getX(),
			bbMin.getY(),
			bbMin.getZ()
		),
		reactphysics3d::Vector3(
			bbMax.getX(),
			bbMax.getY(),
			bbMax.getZ()
		)
	);

	// do the test
	CustomOverlapCallback customOverlapCallback(rigidBodies);
	world.testAABBOverlap(aabb, &customOverlapCallback, typeIds);

	// done
	return rigidBodies.size() > 0;
}

bool World::doesCollideWith(int32_t typeIds, BoundingVolume* boundingVolume, vector<RigidBody*>& rigidBodies)
{
	// do a simple AABB test
	auto boundingBox = boundingVolume->getBoundingBoxTransformed();
	if (doesCollideWith(typeIds, &boundingBox, rigidBodies) == false) {
		return false;
	}

	// done
	return rigidBodies.size() > 0;

	/*
	vector<RigidBody*> rigidBodyCandidates;
	auto boundingBox = boundingVolume->getBoundingBoxTransformed();
	if (doesCollideWith(typeIds, &boundingBox, rigidBodyCandidates) == false) {
		return false;
	}

	// check if they collide
	CollisionResponse response;
	for (auto rigidBody: rigidBodyCandidates) {
		if (rigidBody->doesCollideWith(boundingVolume, &response) == true) {
			if (find(rigidBodies.begin(), rigidBodies.end(), rigidBody) == rigidBodies.end()) {
				rigidBodies.push_back(rigidBody);
			}
		}
	}

	// done
	return rigidBodies.size() > 0;
	*/
}

World* World::clone()
{
	auto clonedWorld = new World();
	for (auto i = 0; i < rigidBodies.size(); i++) {
		auto rigidBody = rigidBodies.at(i);
		// clone obv
		RigidBody* clonedRigidBody = nullptr;
		if (rigidBody->isStatic() == true) {
			// clone static rigid body
			clonedRigidBody = clonedWorld->addStaticRigidBody(rigidBody->id, rigidBody->enabled, rigidBody->typeId, rigidBody->transformations, rigidBody->boundingVolume, rigidBody->getFriction());
		} else {
			// update dynamic rigid body
			clonedRigidBody = clonedWorld->addRigidBody(rigidBody->id, rigidBody->enabled, rigidBody->typeId, rigidBody->transformations, rigidBody->boundingVolume, rigidBody->getRestitution(), rigidBody->getFriction(), rigidBody->getMass(), rigidBody->inverseInertiaMatrix);
		}
		// synch additional properties
		synch(clonedRigidBody, clonedRigidBody);
	}
	return clonedWorld;
}

void World::synch(RigidBody* clonedRigidBody, RigidBody* rigidBody)
{
	Console::println("World::synch()");
	clonedRigidBody->setCollisionTypeIds(rigidBody->getCollisionTypeIds());
	clonedRigidBody->setEnabled(rigidBody->isEnabled());
	clonedRigidBody->setMass(rigidBody->getMass());
	clonedRigidBody->setMass(rigidBody->getMass());
	clonedRigidBody->getAngularVelocity().set(rigidBody->angularVelocity);
	clonedRigidBody->getLinearVelocity().set(rigidBody->linearVelocity);
	clonedRigidBody->fromTransformations(rigidBody->transformations);
}

void World::synch(World* world)
{
	for (auto i = 0; i < rigidBodiesDynamic.size(); i++) {
		auto rigidBody = rigidBodiesDynamic.at(i);
		auto clonedRigidBody = world->getRigidBody(rigidBody->id);
		if (clonedRigidBody == nullptr) {
			Console::println(
				string("Cloned world::entity '") +
				rigidBody->id +
				string("' not found")
			);
			continue;
		}
		// synch rigid bodies
		synch(clonedRigidBody, rigidBody);
	}
}

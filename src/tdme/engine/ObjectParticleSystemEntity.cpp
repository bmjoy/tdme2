#include <tdme/engine/ObjectParticleSystemEntity.h>

#include <vector>
#include <string>

#include <tdme/engine/Engine.h>
#include <tdme/engine/Partition.h>
#include <tdme/engine/Transformations.h>
#include <tdme/engine/primitives/BoundingBox.h>

using std::vector;
using std::string;

using tdme::engine::ObjectParticleSystemEntity;
using tdme::engine::Engine;
using tdme::engine::Partition;
using tdme::engine::Transformations;
using tdme::engine::primitives::BoundingBox;

ObjectParticleSystemEntity::ObjectParticleSystemEntity(const string& id, Model* model, const Vector3& scale, bool autoEmit, bool enableDynamicShadows, int32_t maxCount, ParticleEmitter* emitter) :
	ObjectParticleSystemEntityInternal(id, model, scale, autoEmit, enableDynamicShadows, maxCount, emitter)
{
}

void ObjectParticleSystemEntity::initialize()
{
}

BoundingBox* ObjectParticleSystemEntity::getBoundingBox()
{
	return boundingBox;
}

BoundingBox* ObjectParticleSystemEntity::getBoundingBoxTransformed()
{
	return boundingBoxTransformed;
}

const vector<Object3D*>* ObjectParticleSystemEntity::getEnabledObjects()
{
	return &enabledObjects;
}

void ObjectParticleSystemEntity::fromTransformations(const Transformations& transformations)
{
	ObjectParticleSystemEntityInternal::fromTransformations(transformations);
	if (frustumCulling == true && engine != nullptr && enabled == true) engine->partition->updateEntity(this);
}

void ObjectParticleSystemEntity::update()
{
	ObjectParticleSystemEntityInternal::update();
	if (frustumCulling == true && engine != nullptr && enabled == true) engine->partition->updateEntity(this);
}

void ObjectParticleSystemEntity::setEnabled(bool enabled)
{
	// return if enable state has not changed
	if (this->enabled == enabled) return;
	// frustum culling enabled?
	if (frustumCulling == true) {
		// yeo, add or remove from partition
		if (enabled == true) {
			if (engine != nullptr) engine->partition->addEntity(this);
		} else {
			if (engine != nullptr) engine->partition->removeEntity(this);
		}
	}
	// call parent class::setEnabled()
	ObjectParticleSystemEntityInternal::setEnabled(enabled);
}

void ObjectParticleSystemEntity::updateParticles()
{
	ObjectParticleSystemEntityInternal::updateParticles();
	if (frustumCulling == true && engine != nullptr && enabled == true) engine->partition->updateEntity(this);
}

bool ObjectParticleSystemEntity::isFrustumCulling() {
	return frustumCulling;
}

void ObjectParticleSystemEntity::setFrustumCulling(bool frustumCulling) {
	// check if enabled and engine attached
	if (enabled == true && engine != nullptr) {
		// had frustum culling
		if (this->frustumCulling == true) {
			// yep, remove if set to false now
			if (frustumCulling == false) engine->partition->removeEntity(this);
		} else {
			// yep, add if set to true now
			if (frustumCulling == true) engine->partition->addEntity(this);
		}
	}
	this->frustumCulling = frustumCulling;
}

void ObjectParticleSystemEntity::dispose()
{
	ObjectParticleSystemEntityInternal::dispose();
}

Color4& ObjectParticleSystemEntity::getEffectColorAdd()
{
	return ObjectParticleSystemEntityInternal::getEffectColorAdd();
}

Color4& ObjectParticleSystemEntity::getEffectColorMul()
{
	return ObjectParticleSystemEntityInternal::getEffectColorMul();
}

const string& ObjectParticleSystemEntity::getId()
{
	return ObjectParticleSystemEntityInternal::getId();
}

Vector3& ObjectParticleSystemEntity::getPivot()
{
	return Transformations::getPivot();
}

Rotations* ObjectParticleSystemEntity::getRotations()
{
	return Transformations::getRotations();
}

Vector3& ObjectParticleSystemEntity::getScale()
{
	return Transformations::getScale();
}

const Matrix4x4& ObjectParticleSystemEntity::getTransformationsMatrix() const
{
	return Transformations::getTransformationsMatrix();
}

Vector3& ObjectParticleSystemEntity::getTranslation()
{
	return Transformations::getTranslation();
}

bool ObjectParticleSystemEntity::isDynamicShadowingEnabled()
{
	return ObjectParticleSystemEntityInternal::isDynamicShadowingEnabled();
}

bool ObjectParticleSystemEntity::isEnabled()
{
	return ObjectParticleSystemEntityInternal::isEnabled();
}

bool ObjectParticleSystemEntity::isPickable()
{
	return ObjectParticleSystemEntityInternal::isPickable();
}

void ObjectParticleSystemEntity::setDynamicShadowingEnabled(bool dynamicShadowing)
{
	ObjectParticleSystemEntityInternal::setDynamicShadowingEnabled(dynamicShadowing);
}

void ObjectParticleSystemEntity::setEngine(Engine* engine)
{
	ObjectParticleSystemEntityInternal::setEngine(engine);
}

void ObjectParticleSystemEntity::setPickable(bool pickable)
{
	ObjectParticleSystemEntityInternal::setPickable(pickable);
}

void ObjectParticleSystemEntity::setRenderer(GLRenderer* renderer)
{
	ObjectParticleSystemEntityInternal::setRenderer(renderer);
}

const Transformations& ObjectParticleSystemEntity::getTransformations() const {
	return *this;
}

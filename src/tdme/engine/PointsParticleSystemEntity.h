#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/Transformations.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/model/Color4.h>
#include <tdme/engine/primitives/fwd-tdme.h>
#include <tdme/engine/subsystems/particlesystem/fwd-tdme.h>
#include <tdme/engine/subsystems/renderer/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/engine/subsystems/particlesystem/PointsParticleSystemEntityInternal.h>
#include <tdme/engine/Entity.h>

using std::string;

using tdme::engine::subsystems::particlesystem::PointsParticleSystemEntityInternal;
using tdme::engine::Entity;
using tdme::engine::Engine;
using tdme::engine::Rotations;
using tdme::engine::Transformations;
using tdme::engine::model::Color4;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::subsystems::particlesystem::ParticleEmitter;
using tdme::engine::subsystems::renderer::GLRenderer;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;

/** 
 * Point particle system entity to be used with engine class
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::engine::PointsParticleSystemEntity final
	: public PointsParticleSystemEntityInternal
	, public Entity
{
private:
	bool frustumCulling { true };

public:
	// overriden methods
	void initialize() override;
	BoundingBox* getBoundingBox() override;
	BoundingBox* getBoundingBoxTransformed() override;
	void fromTransformations(const Transformations& transformations) override;
	void update() override;
	void setEnabled(bool enabled) override;
	void updateParticles() override;
	bool isFrustumCulling() override;
	void setFrustumCulling(bool frustumCulling) override;

	/**
	 * Public constructor
	 * @param id
	 * @param do collision tests
	 * @param emitter
	 * @param max points
	 * @param auto emit
	 */
	PointsParticleSystemEntity(const string& id, bool doCollisionTests, ParticleEmitter* emitter, int32_t maxPoints, bool autoEmit);
public:
	// overridden methods
	virtual void dispose() override;
	virtual Color4& getEffectColorAdd() override;
	virtual Color4& getEffectColorMul() override;
	virtual const string& getId() override;
	virtual Vector3& getPivot() override;
	virtual Rotations* getRotations() override;
	virtual Vector3& getScale() override;
	virtual const Matrix4x4& getTransformationsMatrix() const override;
	virtual Vector3& getTranslation() override;
	virtual bool isDynamicShadowingEnabled() override;
	virtual bool isEnabled() override;
	virtual bool isPickable() override;
	virtual void setDynamicShadowingEnabled(bool dynamicShadowing) override;
	virtual void setEngine(Engine* engine) override;
	virtual void setPickable(bool pickable) override;
	virtual void setRenderer(GLRenderer* renderer) override;
	virtual const Transformations& getTransformations() const override;
};

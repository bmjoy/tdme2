#pragma once

#include <tdme/tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/model/Color4.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>
#include <tdme/tools/shared/model/fwd-tdme.h>

using tdme::engine::model::Color4;
using tdme::math::Vector3;

/** 
 * Bounding box particle emitter 
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::tools::shared::model::LevelEditorEntityParticleSystem_BoundingBoxParticleEmitter final
{
private:
	int32_t count;
	int64_t lifeTime;
	int64_t lifeTimeRnd;
	float mass;
	float massRnd;
	Vector3 velocity;
	Vector3 velocityRnd;
	Color4 colorStart;
	Color4 colorEnd;
	Vector3 obbCenter;
	Vector3 obbHalfextension;
	Vector3 obbAxis0;
	Vector3 obbAxis1;
	Vector3 obbAxis2;

public:

	/** 
	 * @return count
	 */
	inline int32_t getCount() {
		return count;
	}

	/** 
	 * Set count
	 * @param count count
	 */
	inline void setCount(int32_t count) {
		this->count = count;
	}

	/** 
	 * @return life time
	 */
	inline int64_t getLifeTime() {
		return lifeTime;
	}

	/** 
	 * Set life time
	 * @param lifeTime life time
	 */
	inline void setLifeTime(int64_t lifeTime) {
		this->lifeTime = lifeTime;
	}

	/** 
	 * @return life time rnd
	 */
	inline int64_t getLifeTimeRnd() {
		return lifeTimeRnd;
	}

	/** 
	 * Set life time rnd
	 * @param lifeTimeRnd life time rnd
	 */
	inline void setLifeTimeRnd(int64_t lifeTimeRnd) {
		this->lifeTimeRnd = lifeTimeRnd;
	}

	/** 
	 * @return mass
	 */
	inline float getMass() {
		return mass;
	}

	/** 
	 * Set mass
	 * @param mass mass
	 */
	inline void setMass(float mass) {
		this->mass = mass;
	}

	/** 
	 * @return mass rnd
	 */
	inline float getMassRnd() {
		return massRnd;
	}

	/** 
	 * Set mass rnd
	 * @param massRnd mass rnd
	 */
	inline void setMassRnd(float massRnd) {
		this->massRnd = massRnd;
	}

	/** 
	 * @return velocity
	 */
	inline Vector3& getVelocity() {
		return velocity;
	}

	/** 
	 * @return velocity rnd
	 */
	inline Vector3& getVelocityRnd() {
		return velocityRnd;
	}

	/** 
	 * @return color start
	 */
	inline Color4& getColorStart() {
		return colorStart;
	}

	/** 
	 * @return color end
	 */
	inline Color4& getColorEnd() {
		return colorEnd;
	}

	/** 
	 * @return obb center
	 */
	inline Vector3& getObbCenter() {
		return obbCenter;
	}

	/** 
	 * @return obb half extension
	 */
	inline Vector3& getObbHalfextension() {
		return obbHalfextension;
	}

	/** 
	 * @return obb axis 0
	 */
	inline Vector3& getObbAxis0() {
		return obbAxis0;
	}

	/** 
	 * @return obb axis 1
	 */
	inline Vector3& getObbAxis1() {
		return obbAxis1;
	}

	/** 
	 * @return obb axis 2
	 */
	inline Vector3& getObbAxis2() {
		return obbAxis2;
	}

	/**
	 * Public constructor
	 */
	LevelEditorEntityParticleSystem_BoundingBoxParticleEmitter();
};

#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/Transformations.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/model/Color4.h>
#include <tdme/engine/primitives/fwd-tdme.h>
#include <tdme/engine/subsystems/renderer/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>

using std::string;

using tdme::engine::Engine;
using tdme::engine::Transformations;
using tdme::engine::model::Color4;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;

/** 
 * TDME engine entity
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::engine::Entity
{
	friend class Engine;
	friend class EntityHierarchy;
	friend class ParticleSystemGroup;

private:
	/**
	 * @return parent entity
	 */
	virtual Entity* getParentEntity() = 0;

	/**
	 * Set parent entity, needs to be called before adding to engine
	 * @param entity entity
	 */
	virtual void setParentEntity(Entity* entity) = 0;

	/**
	 * Apply parent transformations
	 * @param parentTransformations parent transformations
	 */
	virtual void applyParentTransformations(const Transformations& parentTransformations) = 0;

public:

	/** 
	 * Set up engine
	 * @param engine engine
	 */
	virtual void setEngine(Engine* engine) = 0;

	/** 
	 * Set up renderer
	 * @param renderer renderer
	 */
	virtual void setRenderer(Renderer* renderer) = 0;

	/** 
	 * @return object id
	 */
	virtual const string& getId() = 0;

	/** 
	 * @return true if enabled to be rendered
	 */
	virtual bool isEnabled() = 0;

	/** 
	 * Enable/disable rendering
	 * @param enabled enabled
	 */
	virtual void setEnabled(bool enabled) = 0;

	/**
	 * @return if frustum culling is enabled
	 */
	virtual bool isFrustumCulling() = 0;

	/**
	 * Set frustum culling
	 * @param frustumCulling frustum culling
	 */
	virtual void setFrustumCulling(bool frustumCulling) = 0;

	/** 
	 * @return if object is pickable
	 */
	virtual bool isPickable() = 0;

	/** 
	 * Set this object pickable
	 * @param pickable pickable
	 */
	virtual void setPickable(bool pickable) = 0;

	/** 
	 * @return if entity contributes to shadows
	 */
	virtual bool isContributesShadows() = 0;

	/** 
	 * Enable/disable contributes shadows
	 * @param contributesShadows contributes shadows
	 */
	virtual void setContributesShadows(bool contributesShadows) = 0;

	/** 
	 * @return if entity receives shadows
	 */
	virtual bool isReceivesShadows() = 0;

	/**
	 * Enable/disable receives shadows
	 * @param receivesShadows receives shadows
	 */
	virtual void setReceivesShadows(bool receivesShadows) = 0;

	/**
	 * The effect color will be multiplied with fragment color
	 * @return effect color
	 */
	virtual const Color4& getEffectColorMul() const = 0;

	/** 
	 * Set effect color that will be multiplied with fragment color
	 * @param effectColorMul effect color
	 */
	virtual void setEffectColorMul(const Color4& effectColorMul) = 0;

	/**
	 * The effect color will be added to fragment color
	 * @return effect color
	 */
	virtual const Color4& getEffectColorAdd() const = 0;

	/**
	 * Set effect color that will be added to fragment color
	 * @return effect color
	 */
	virtual void setEffectColorAdd(const Color4& effectColorAdd) = 0;

	/** 
	 * Initiates this object 3d 
	 */
	virtual void initialize() = 0;

	/** 
	 * Dispose this object 3d
	 */
	virtual void dispose() = 0;

	/** 
	 * @return bounding box / in model coordinate space
	 */
	virtual BoundingBox* getBoundingBox() = 0;

	/** 
	 * @return bounding box transformed / in world coordinate space
	 */
	virtual BoundingBox* getBoundingBoxTransformed() = 0;

	/**
	 * @return object translation
	 */
	virtual const Vector3& getTranslation() const = 0;

	/**
	 * Set translation
	 * @param translation translation
	 */
	virtual void setTranslation(const Vector3& translation) = 0;

	/**
	 * @return object scale
	 */
	virtual const Vector3& getScale() const = 0;

	/**
	 * Set scale
	 * @param scale scale
	 */
	virtual void setScale(const Vector3& scale) = 0;

	/**
	 * @return pivot or center of rotations
	 */
	virtual const Vector3& getPivot() const = 0;

	/**
	 * Set pivot
	 * @param pivot pivot
	 */
	virtual void setPivot(const Vector3& pivot) = 0;

	/**
	 * @return rotation count
	 */
	virtual const int getRotationCount() const = 0;

	/**
	 * Get rotation at given index
	 * @param idx rotation index
	 * @return rotation
	 */
	virtual Rotation& getRotation(const int idx) = 0;

	/**
	 * Add rotation
	 * @param axis axis
	 * @param angle angle
	 */
	virtual void addRotation(const Vector3& axis, const float angle) = 0;

	/**
	 * Remove rotation
	 * @param idx index
	 */
	virtual void removeRotation(int idx) = 0;

	/**
	 * @param idx rotation index
	 * @return rotation axis for rotation with given index
	 */
	virtual const Vector3& getRotationAxis(const int idx) const = 0;

	/**
	 * Set rotation axis
	 * @param idx rotation index
	 * @param axis rotation axis
	 */
	virtual void setRotationAxis(const int idx, const Vector3& axis) = 0;

	/**
	 * @param idx rotation index
	 * @return rotation angle for rotation with given index
	 */
	virtual const float getRotationAngle(const int idx) const = 0;

	/**
	 * @param idx rotation index
	 * @param angle rotation angle
	 * @return rotation angle for rotation with given index
	 */
	virtual void setRotationAngle(const int idx, const float angle) = 0;

	/**
	 * @return rotations quaternion
	 */
	virtual const Quaternion& getRotationsQuaternion() const = 0;

	/**
	 * @return this transformations matrix
	 */
	virtual const Matrix4x4& getTransformationsMatrix() const = 0;

	/**
	 * Set up this transformations from given transformations
	 * @param transformations transformations
	 */
	virtual void fromTransformations(const Transformations& transformations) = 0;

	/** 
	 * Update transformations
	 */
	virtual void update() = 0;

	/**
	 * @return this transformations
	 */
	virtual const Transformations& getTransformations() const = 0;

	/**
	 * Destructor
	 */
	virtual ~Entity() {}
};

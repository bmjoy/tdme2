#pragma once

#include <vector>

#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/Rotation.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Quaternion.h>

using std::vector;

using tdme::engine::Rotation;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;
using tdme::math::Quaternion;

/** 
 * Transformations
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::engine::Transformations {
private:
	Vector3 translation {  };
	Vector3 scale {  };
	Vector3 pivot {  };
	Quaternion rotationsQuaternion { };
	vector<Rotation> rotations {  };
	Matrix4x4 transformationsMatrix {  };

public:

	/** 
	 * @return object translation
	 */
	inline virtual const Vector3& getTranslation() const {
		return translation;
	}

	/** 
	 * Set translation
	 * @param translation
	 */
	inline virtual void setTranslation(const Vector3& translation) {
		this->translation.set(translation);
	}

	/**
	 * @return object scale
	 */
	inline virtual const Vector3& getScale() const {
		return scale;
	}

	/** 
	 * Set scale
	 * @param scale
	 */
	inline virtual void setScale(const Vector3& scale) {
		this->scale.set(scale);
	}

	/**
	 * @return pivot or center of rotations
	 */
	inline virtual const Vector3& getPivot() const {
		return pivot;
	}

	/**
	 * Set pivot
	 * @param pivot
	 */
	inline virtual void setPivot(const Vector3& pivot) {
		this->pivot.set(pivot);
	}

	/**
	 * @return rotation count
	 */
	inline const virtual int getRotationCount() const {
		return rotations.size();
	}

	/**
	 * Get rotation at given index
	 * @param rotation index
	 * @return rotation
	 */
	inline virtual Rotation& getRotation(const int idx) {
		return rotations.at(idx);
	}

	/**
	 * Add rotation
	 * @param axis
	 * @param angle
	 */
	inline virtual void addRotation(const Vector3& axis, const float angle) {
		rotations.push_back(Rotation(angle, axis));
	}

	/**
	 * Remove rotation
	 * @param index
	 */
	inline virtual void removeRotation(const int idx) {
		rotations.erase(rotations.begin() + idx);
	}

	/** 
	 * @param rotation index
	 * @return rotation axis for rotation with given index
	 */
	inline virtual const Vector3& getRotationAxis(const int idx) const {
		return rotations[idx].getAxis();
	}

	/** 
	 * Set rotation axis
	 * @param rotation index
	 * @param rotation axis
	 */
	inline virtual void setRotationAxis(const int idx, const Vector3& axis) {
		return rotations[idx].setAxis(axis);
	}

	/**
	 * @param rotation index
	 * @return rotation angle for rotation with given index
	 */
	inline virtual const float getRotationAngle(const int idx) const {
		return rotations[idx].getAngle();
	}

	/**
	 * @param rotation index
	 * @param rotation angle
	 * @return rotation angle for rotation with given index
	 */
	inline virtual void setRotationAngle(const int idx, const float angle) {
		rotations[idx].setAngle(angle);
	}

	/**
	 * @return rotations quaternion
	 */
	inline virtual const Quaternion& getRotationsQuaternion() const {
		return rotationsQuaternion;
	}

	/**
	 * @return this transformations matrix
	 */
	inline virtual const Matrix4x4& getTransformationsMatrix() const {
		return transformationsMatrix;
	}

	/** 
	 * Set up this transformations from given transformations
	 * @param transformations
	 */
	virtual void fromTransformations(const Transformations& transformations);

	/** 
	 * Computes transformation matrix
	 */
	virtual void update();

	/**
	 * Invert this transformations
	 */
	virtual void invert();

	/**
	 * Public constructor
	 */
	Transformations();

	/**
	 * Destructor
	 */
	virtual ~Transformations();
};

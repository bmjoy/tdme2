#pragma once

#include <array>

#include <tdme/tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/math/Math.h>

using std::array;

using tdme::math::Math;

/** 
 * Color 4 base definition
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::engine::model::Color4Base
{
protected:
	array<float, 4> data {  };

public:
	/**
	 * Set up color
	 * @param color float array
	 */
	inline virtual void set(const array<float, 4>& color) {
		this->data = color;
	}

	/** 
	 * Set up color
	 * @param red
	 * @param green
	 * @param blue
	 * @param alpha
	 */
	inline virtual void set(float r, float g, float b, float a) {
		data[0] = r;
		data[1] = g;
		data[2] = b;
		data[3] = a;
	}

	/** 
	 * Sets up this color with given color
	 * @param color
	 */
	inline virtual void set(const Color4Base& color) {
		data = color.data;
	}

	/** 
	 * Add to color
	 * @param red
	 * @param green
	 * @param blue
	 * @param alpha
	 */
	inline virtual void add(float r, float g, float b, float a) {
		data[0] += r;
		data[1] += g;
		data[2] += b;
		data[3] += a;
	}

	/** 
	 * @return red component
	 */
	inline virtual float getRed() const {
		return data[0];
	}

	/** 
	 * @param red component
	 */
	inline virtual void setRed(float red) {
		data[0] = red;
	}

	/** 
	 * @return green component
	 */
	inline virtual float getGreen() const {
		return data[1];
	}

	/** 
	 * @param green component
	 */
	inline virtual void setGreen(float green) {
		data[1] = green;
	}

	/** 
	 * @return blue component
	 */
	inline virtual float getBlue() const {
		return data[2];
	}

	/** 
	 * @param blue component
	 */
	inline virtual void setBlue(float blue) {
		data[2] = blue;
	}

	/** 
	 * @return alpha component
	 */
	inline virtual float getAlpha() const {
		return data[3];
	}

	/** 
	 * @param alpha component
	 */
	inline virtual void setAlpha(float alpha) {
		data[3] = alpha;
	}

	/** 
	 * @return rgba as float array 
	 */
	inline virtual array<float, 4>& getArray() const {
		return (array<float, 4>&)data;
	}

	/** 
	 * Compares this color with given color
	 * @return equality
	 */
	inline virtual bool equals(const Color4Base& c) const {
		return (this == &c) || (
			Math::abs(data[0] - c.data[0]) < Math::EPSILON &&
			Math::abs(data[1] - c.data[1]) < Math::EPSILON &&
			Math::abs(data[2] - c.data[2]) < Math::EPSILON &&
			Math::abs(data[3] - c.data[3]) < Math::EPSILON
		);
	}

	/**
	 * Public constructor
	 */
	inline Color4Base() {
		data[0] = 0.0f;
		data[1] = 0.0f;
		data[2] = 0.0f;
		data[3] = 1.0f;
	}

	/**
	 * Public constructor
	 * @param color
	 */
	inline Color4Base(const Color4Base& color) {
		data = color.data;
	}

	/**
	 * Public constructor
	 * @param color
	 */
	inline Color4Base(const array<float, 4>& color) {
		this->data = color;
	}

	/**
	 * Public constructor
	 * @param red
	 * @param green
	 * @param blue
	 * @param alpha
	 */
	inline Color4Base(float r, float g, float b, float a) {
		data[0] = r;
		data[1] = g;
		data[2] = b;
		data[3] = a;
	}

};


#pragma once

#include <tdme/utils/fwd-tdme.h>

#include <cmath>
#include <limits>
#include <string>

using std::isnan;
using std::isfinite;
using std::numeric_limits;
using std::string;

/**
 * Float class
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::utils::Float final
{
public:
	static constexpr float MAX_VALUE { numeric_limits<float>::max() };
	static constexpr float MIN_VALUE { -numeric_limits<float>::max() };
	static constexpr float NaN { numeric_limits<float>::quiet_NaN() };
	static constexpr int32_t SIZE { 32 };

	/**
	 * Parse float
	 * @param str string
	 * @return float
	 */
	static float parseFloat(const string& str);

	/**
	 * Check if float is not a number
	 * @param value float value
	 * @return if value is not a number
	 */
	inline static bool isNaN(float value) {
		return isnan(value);
	}

	/**
	 * Check if float is finite
	 * @param value float value
	 * @return if value is finite
	 */
	inline static bool isFinite(float value) {
		return isfinite(value);
	}

	/**
	 * Interpolates between float 1 and float 2 by 0f<=t<=1f linearly
	 * @param f1 float 1
	 * @param f2 float 2
	 * @param t t
	 * @return interpolated float value
	 */
	inline static float interpolateLinear(float f1, float f2, float t) {
		return (f2 * t) + ((1.0f - t) * f1);
	}


};

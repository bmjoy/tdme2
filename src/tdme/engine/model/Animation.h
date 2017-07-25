// Generated from /tdme/src/tdme/engine/model/Animation.java

#pragma once

#include <vector>

#include <fwd-tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>

using std::vector;

using tdme::math::Matrix4x4;

/** 
 * AnimationSetup
 * @author andreas.drewke
 * @version $Id$
 */
class tdme::engine::model::Animation final
{
private:
	int32_t frames {  };
	vector<tdme::math::Matrix4x4> transformationsMatrices {  };

public:

	/** 
	 * @return number of frames
	 */
	int32_t getFrames();

	/** 
	 * Returns transformation matrices
	 * @return transformation matrices
	 */
	vector<Matrix4x4>* getTransformationsMatrices();

	// Generated
	Animation(int32_t frames);
};

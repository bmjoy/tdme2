#pragma once

#include <array>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/Engine.h>
#include <tdme/engine/subsystems/lighting/fwd-tdme.h>
#include <tdme/engine/subsystems/lighting/LightingShaderConstants.h>
#include <tdme/engine/subsystems/lighting/LightingShaderImplementation.h>
#include <tdme/engine/subsystems/renderer/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Matrix4x4.h>

using std::array;

using tdme::engine::Engine;
using tdme::engine::subsystems::lighting::LightingShaderConstants;
using tdme::engine::subsystems::lighting::LightingShaderImplementation;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::math::Matrix4x4;

/** 
 * Lighting shader implementation
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::engine::subsystems::lighting::LightingShaderPBRBaseImplementation: public LightingShaderImplementation
{
protected:
	int32_t renderLightingProgramId { -1 };
	int32_t renderLightingFragmentShaderId { -1 };
	int32_t renderLightingVertexShaderId { -1 };
	int32_t uniformBaseColorFactor { -1 };
	int32_t uniformBaseColorSampler { -1 };
	int32_t uniformBaseColorSamplerAvailable { -1 };
	int32_t uniformCamera { -1 };
	int32_t uniformExposure { -1 };
	int32_t uniformMetallicFactor { -1 };
	int32_t uniformMetallicRoughnessSampler { -1 };
	int32_t uniformMetallicRoughnessSamplerAvailable { -1 };
	int32_t uniformNormalSampler { -1 };
	int32_t uniformNormalSamplerAvailable { -1 };
	int32_t uniformNormalScale { -1 };
	int32_t uniformRoughnessFactor { -1 };
	int32_t uniformViewProjectionMatrix { -1 };
	array<int32_t, Engine::LIGHTS_MAX> uniformLightEnabled;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightDirection;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightRange;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightColor;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightIntensity;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightPosition;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightInnerConeCos;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightOuterConeCos;
	array<int32_t, Engine::LIGHTS_MAX> uniformLightType;

	bool initialized { false };
	Renderer* renderer { nullptr };
public:

	// overriden methods
	virtual bool isInitialized() override;
	virtual void initialize() override;
	virtual void useProgram(Engine* engine, void* context) override;
	virtual void unUseProgram(void* context) override;
	virtual void updateEffect(Renderer* renderer, void* context) override;
	virtual void updateMaterial(Renderer* renderer, void* context) override;
	virtual void updateLight(Renderer* renderer, void* context, int32_t lightId) override;
	virtual void updateMatrices(Renderer* renderer, void* context) override;
	virtual void updateTextureMatrix(Renderer* renderer, void* context) override;
	virtual void bindTexture(Renderer* renderer, void* context, int32_t textureId) override;

	/**
	 * Public constructor
	 * @param renderer renderer
	 */
	LightingShaderPBRBaseImplementation(Renderer* renderer);
};

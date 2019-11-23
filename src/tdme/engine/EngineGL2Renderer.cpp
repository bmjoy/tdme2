#include <tdme/engine/Engine.h>
#include <tdme/engine/subsystems/earlyzrejection/EZRShaderPre.h>
#include <tdme/engine/subsystems/lighting/LightingShader.h>
#include <tdme/engine/subsystems/lines/LinesShader.h>
#include <tdme/engine/subsystems/particlesystem/ParticlesShader.h>
#include <tdme/engine/subsystems/shadowmapping/ShadowMapping.h>
#include <tdme/gui/renderer/GUIShader.h>
#include <tdme/engine/EngineGL2Renderer.h>

using tdme::engine::EngineGL2Renderer;
using tdme::engine::Engine;
using tdme::engine::subsystems::earlyzrejection::EZRShaderPre;
using tdme::engine::subsystems::lighting::LightingShader;
using tdme::engine::subsystems::lines::LinesShader;
using tdme::engine::subsystems::particlesystem::ParticlesShader;
using tdme::engine::subsystems::shadowmapping::ShadowMapping;
using tdme::gui::renderer::GUIShader;

EngineGL2Renderer::EngineGL2Renderer(Engine *engine) : engine(engine)
{
}

void EngineGL2Renderer::onUpdateProjectionMatrix(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateMatrices(this, context);

	if (Engine::particlesShader != nullptr)
		Engine::particlesShader->updateMatrices(this, context);

	if (Engine::linesShader != nullptr)
		Engine::linesShader->updateMatrices(this, context);

	if (Engine::Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateMatrices(this, context);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->updateMatrices(this, context);
}

void EngineGL2Renderer::onUpdateCameraMatrix(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateMatrices(this, context);

	if (Engine::particlesShader != nullptr)
		Engine::particlesShader->updateMatrices(this, context);

	if (Engine::linesShader != nullptr)
		Engine::linesShader->updateMatrices(this, context);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateMatrices(this, context);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->updateMatrices(this, context);
}

void EngineGL2Renderer::onUpdateModelViewMatrix(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateMatrices(this, context);

	if (Engine::particlesShader != nullptr)
		Engine::particlesShader->updateMatrices(this, context);

	if (Engine::linesShader != nullptr)
		Engine::linesShader->updateMatrices(this, context);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateMatrices(this, context);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->updateMatrices(this, context);
}

void EngineGL2Renderer::onBindTexture(void* context, int32_t textureId)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->bindTexture(this, context, textureId);

	if (Engine::guiShader != nullptr)
		Engine::guiShader->bindTexture(this, textureId);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->bindTexture(this, context, textureId);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->bindTexture(this, context, textureId);
}

void EngineGL2Renderer::onUpdateTextureMatrix(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateTextureMatrix(this, context);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateTextureMatrix(this, context);

	if (Engine::guiShader != nullptr)
		Engine::guiShader->updateTextureMatrix(this);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->updateTextureMatrix(this, context);
}

void EngineGL2Renderer::onUpdateEffect(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateEffect(this, context);

	if (Engine::particlesShader != nullptr)
		Engine::particlesShader->updateEffect(this, context);

	if (Engine::linesShader != nullptr)
		Engine::linesShader->updateEffect(this, context);

	if (Engine::guiShader != nullptr)
		Engine::guiShader->updateEffect(this);
}

void EngineGL2Renderer::onUpdateLight(void* context, int32_t lightId)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateLight(this, context, lightId);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateLight(this, context, lightId);
}

void EngineGL2Renderer::onUpdateMaterial(void* context)
{
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->updateMaterial(this, context);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->updateMaterial(this, context);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->updateMaterial(this, context);
}

void EngineGL2Renderer::onUpdateShader(void* context) {
	if (Engine::lightingShader != nullptr)
		Engine::lightingShader->setShader(context, shader);

	if (Engine::currentEngine->shadowMapping != nullptr)
		Engine::currentEngine->shadowMapping->setShader(context, shader);

	if (Engine::ezrShaderPre != nullptr)
		Engine::ezrShaderPre->setShader(context, shader);
}

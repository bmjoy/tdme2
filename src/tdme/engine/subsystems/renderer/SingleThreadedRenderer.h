#pragma once

#include <array>

#include <tdme/tdme.h>
#include <tdme/engine/subsystems/renderer/fwd-tdme.h>
#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/engine/subsystems/renderer/Renderer_Light.h>
#include <tdme/engine/subsystems/renderer/Renderer_Material.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Matrix2D3x3.h>
#include <tdme/math/Matrix4x4.h>

using std::array;

using tdme::engine::subsystems::renderer::Renderer;
using tdme::engine::subsystems::renderer::Renderer_Light;
using tdme::engine::subsystems::renderer::Renderer_Material;
using tdme::math::Matrix2D3x3;
using tdme::math::Matrix4x4;

/** 
 * Single threaded renderer
 * @author Andreas Drewke
 * @ersion $Id$
 */
class tdme::engine::subsystems::renderer::SingleThreadedRenderer: public Renderer
{
private:
	array<float, 4> effectColorMul {{ 1.0f, 1.0f, 1.0f, 1.0f }};
	array<float, 4> effectColorAdd {{ 0.0f, 0.0f, 0.0f, 0.0f }};
	Renderer_Material material;
	array<Renderer_Light, 8> lights;

protected:
	int activeTextureUnit;
	Matrix4x4 projectionMatrix;
	Matrix4x4 cameraMatrix;
	Matrix4x4 modelViewMatrix;
	Matrix4x4 viewportMatrix;
	Matrix2D3x3 textureMatrix;

public:
	virtual Matrix4x4& getProjectionMatrix();
	virtual Matrix4x4& getCameraMatrix();
	virtual Matrix4x4& getModelViewMatrix();
	virtual Matrix4x4& getViewportMatrix();
	virtual Matrix2D3x3& getTextureMatrix(void* context);
	virtual const Renderer_Light& getLight(void* context, int32_t lightId);
	virtual void setLight(void* context, int32_t lightId, const Renderer_Light& light);
	virtual const array<float, 4>& getEffectColorMul(void* context);
	virtual void setEffectColorMul(void* context, const array<float, 4>& effectColorMul);
	virtual const array<float, 4>& getEffectColorAdd(void* context);
	virtual void setEffectColorAdd(void* context, const array<float, 4>& effectColorAdd);
	virtual const Renderer_Material& getMaterial(void* context);
	virtual void setMaterial(void* context, const Renderer_Material& material);

	/**
	 * Public constructor
	 */
	SingleThreadedRenderer();

	/**
	 * Destructor
	 */
	virtual ~SingleThreadedRenderer();

private:

	//
	friend class Renderer_Material;
	friend class Renderer_Light;
};
#include <tdme/engine/Camera.h>

#include <tdme/engine/Frustum.h>
#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/math/Math.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Vector3.h>
#include <tdme/utils/Console.h>

using tdme::engine::Camera;
using tdme::math::Math;
using tdme::engine::Frustum;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;
using tdme::utils::Console;

Camera::Camera(Renderer* renderer)
{
	this->renderer = renderer;
	width = 0;
	height = 0;
	aspect = 1;
	fovY = 45.0f;
	zNear = 10.0f;
	zFar = 4000.0f;
	cameraMode = CAMERAMODE_LOOKAT;
	upVector.set(0.0f, 1.0f, 0.0f);
	forwardVector.set(0.0f, 0.0f, 1.0f);
	sideVector.set(1.0f, 0.0f, 0.0f);
	lookFrom.set(0.0f, 50.0f, 400.0f);
	lookAt.set(0.0f, 50.0f, 0.0f);
	frustum = new Frustum(renderer);
	lastZFar = -1.0f;
	lastZNear = -1.0f;
	frustumChanged = true;
}

Camera::~Camera() {
	delete frustum;
}

Vector3 Camera::defaultUp(0.0f, 1.0f, 0.0f);

Vector3 Camera::computeUpVector(const Vector3& lookFrom, const Vector3& lookAt)
{
	Vector3 tmpForward;
	Vector3 tmpSide;
	Vector3 tmpUpVector;
	tmpForward.set(lookAt).sub(lookFrom).normalize();
	if (Math::abs(tmpForward.getX()) < Math::EPSILON && Math::abs(tmpForward.getZ()) < Math::EPSILON) {
		tmpUpVector.set(0.0f, 0.0f, tmpForward.getY()).normalize();
		return tmpUpVector;
	}
	Vector3::computeCrossProduct(tmpForward, defaultUp, tmpSide).normalize();
	Vector3::computeCrossProduct(tmpSide, tmpForward, tmpUpVector).normalize();
	return tmpUpVector;
}

Matrix4x4& Camera::computeProjectionMatrix()
{
	// see: see http://www.songho.ca/opengl/gl_transform.html
	auto tangent = static_cast< float >(Math::tan(fovY / 2.0f * 3.1415927f / 180.0f));
	auto height = zNear * tangent;
	auto width = height * aspect;
	return computeFrustumMatrix(-width, width, -height, height, zNear, zFar);
}

Matrix4x4& Camera::computeFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
{
	// see: http://www.songho.ca/opengl/gl_transform.html
	return projectionMatrix.set(
		2.0f * near / (right - left),
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		2.0f * near / (top - bottom),
		0.0f,
		0.0f,
		(right + left) / (right - left),
		(top + bottom) / (top - bottom),
		-(far + near) / (far - near),
		-1.0f,
		0.0f,
		0.0f,
		-(2.0f * far * near) / (far - near),
		1.0f
	);
}

Matrix4x4& Camera::computeModelViewMatrix()
{
	Matrix4x4 tmpAxesMatrix;
	Vector3 tmpUp;
	Vector3 tmpLookFromInverted;
	if (cameraMode == CAMERAMODE_LOOKAT) {
		forwardVector.set(lookAt).sub(lookFrom).normalize();
		Vector3::computeCrossProduct(forwardVector, upVector, sideVector).normalize();
	}
	Vector3::computeCrossProduct(sideVector, forwardVector, tmpUp);
	modelViewMatrix.
		identity().
		translate(
			tmpLookFromInverted.set(lookFrom).scale(-1.0f)
		).
		multiply(
			tmpAxesMatrix.set(
				sideVector[0],
				tmpUp[0],
				-forwardVector[0],
				0.0f,
				sideVector[1],
				tmpUp[1],
				-forwardVector[1],
				0.0f,
				sideVector[2],
				tmpUp[2],
				-forwardVector[2],
				0.0f,
				0.0f,
				0.0f,
				0.0f,
				1.0f
			)
		);
	return modelViewMatrix;
}

void Camera::update(void* context, int32_t width, int32_t height)
{
	// setup new view port if required
	auto reshaped = false;
	if (this->width != width || this->height != height) {
		reshaped = true;
		if (height <= 0)
			height = 1;

		aspect = static_cast< float >(width) / static_cast< float >(height);
		this->width = width;
		this->height = height;
		renderer->getViewportMatrix().set(
			width / 2.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			height / 2.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
			0.0f,
			0 + (width / 2.0f),
			0 + (height / 2.0f),
			0.0f,
			1.0f
		);
	}

	// setup projection and model view and such
	renderer->getProjectionMatrix().set(computeProjectionMatrix());
	renderer->onUpdateProjectionMatrix(context);
	renderer->getModelViewMatrix().set(computeModelViewMatrix());
	renderer->onUpdateModelViewMatrix(context);
	renderer->getCameraMatrix().set(renderer->getModelViewMatrix());
	renderer->onUpdateCameraMatrix(context);

	//
	mvpInvertedMatrix.set(modelViewMatrix).multiply(projectionMatrix).invert();
	mvpMatrix.set(modelViewMatrix).multiply(projectionMatrix);

	frustumChanged =
		reshaped == true ||
		lastZNear != zNear ||
		lastZFar != zFar ||
		lastForwardVector.equals(forwardVector) == false ||
		lastSideVector.equals(sideVector) == false ||
		lastLookFrom.equals(lookFrom) == false;

	if (frustumChanged == true) {
		// update frustum
		frustum->updateFrustum();
	}

	lastZNear = zNear;
	lastZFar = zFar;
	lastForwardVector.set(forwardVector);
	lastSideVector.set(sideVector);
	lastLookFrom.set(lookFrom);
}


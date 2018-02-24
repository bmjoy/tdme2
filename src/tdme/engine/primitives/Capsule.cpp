#include <tdme/engine/primitives/Capsule.h>

#include <tdme/math/Math.h>
#include <tdme/engine/Transformations.h>
#include <tdme/engine/physics/CollisionDetection.h>
#include <tdme/engine/primitives/BoundingBox.h>
#include <tdme/engine/primitives/BoundingVolume.h>
#include <tdme/engine/primitives/ConvexMesh.h>
#include <tdme/engine/primitives/OrientedBoundingBox.h>
#include <tdme/engine/primitives/Sphere.h>
#include <tdme/engine/primitives/Triangle.h>
#include <tdme/math/Matrix4x4.h>
#include <tdme/math/Vector3.h>

using tdme::engine::primitives::Capsule;

using tdme::math::Math;
using tdme::engine::Object3DModel;
using tdme::engine::Transformations;
using tdme::engine::physics::CollisionDetection;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::primitives::BoundingVolume;
using tdme::engine::primitives::ConvexMesh;
using tdme::engine::primitives::OrientedBoundingBox;
using tdme::engine::primitives::Sphere;
using tdme::engine::primitives::Triangle;
using tdme::math::Matrix4x4;
using tdme::math::Vector3;

Capsule::Capsule(const Vector3& a, const Vector3& b, float radius)
{
	this->a.set(a);
	this->b.set(b);
	this->radius = radius;
	this->convexMeshA.set(a);
	this->convexMeshB.set(b);
	this->convexMeshRadius = radius;
	createConvexMesh();
	update();
}

void Capsule::createConvexMesh() {
	int segmentsX = 5;
	int segmentsY = 5;
	vector<Triangle> triangles;
	vector<Vector3> vertices;
	Quaternion rotationQuaternion;
	rotationQuaternion.identity();
	Vector3 yAxis(0.0f, -1.0f, 0.0f);
	Vector3 abNormalized = a.clone().sub(b).normalize();
	auto& abNormalizedVectorXYZ = abNormalized.getArray();
	Vector3 rotationAxis;
	if (Math::abs(abNormalizedVectorXYZ[0]) < Math::EPSILON && Math::abs(abNormalizedVectorXYZ[2]) < Math::EPSILON) {
		rotationAxis.set(abNormalizedVectorXYZ[1], 0.0f, 0.0f);
	} else {
		Vector3::computeCrossProduct(yAxis, abNormalized, rotationAxis).normalize();
	}
	auto angle = Vector3::computeAngle(yAxis, abNormalized, yAxis);
	rotationQuaternion.rotate(angle, rotationAxis);
	vertices.resize((segmentsY + 2) * segmentsX);
	for (auto ySegment = segmentsY / 2; ySegment <= segmentsY; ySegment++)
	for (auto xSegment = 0; xSegment < segmentsX; xSegment++) {
		auto vertex = Vector3();
		rotationQuaternion.multiply(
			Vector3(
				((Math::sin(Math::PI * ySegment / segmentsY) * Math::cos(Math::PI * 2 * xSegment / segmentsX))),
				((Math::cos(Math::PI * ySegment / segmentsY))),
				((Math::sin(Math::PI * ySegment / segmentsY) * Math::sin(Math::PI * 2 * xSegment / segmentsX)))
			),
			vertex
		);
		vertex.scale(radius);
		vertex.add(a);
		vertices[ySegment * segmentsX + xSegment] = vertex;
	}
	for (auto ySegment = 0; ySegment <= segmentsY / 2; ySegment++)
	for (auto xSegment = 0; xSegment < segmentsX; xSegment++) {
		auto vertex = Vector3();
		rotationQuaternion.multiply(
			Vector3(
				((Math::sin(Math::PI * ySegment / segmentsY) * Math::cos(Math::PI * 2 * xSegment / segmentsX))),
				((Math::cos(Math::PI * ySegment / segmentsY))),
				((Math::sin(Math::PI * ySegment / segmentsY) * Math::sin(Math::PI * 2 * xSegment / segmentsX)))
			),
			vertex
		);
		vertex.scale(radius);
		vertex.add(b);
		vertices[ySegment * segmentsX + xSegment] = vertex;
	}
	int vi0, vi1, vi2;
	for (auto y = 0; y <= segmentsY + 1; y++) {
		for (auto x = 0; x < segmentsX; x++) {
			vi0 = ((y + 0) % (segmentsY + 1)) * segmentsX + ((x + 0) % (segmentsX));
			vi1 = ((y + 1) % (segmentsY + 1)) * segmentsX + ((x + 1) % (segmentsX));
			vi2 = ((y + 1) % (segmentsY + 1)) * segmentsX + ((x + 0) % (segmentsX));
			triangles.push_back(Triangle(vertices[vi0], vertices[vi1], vertices[vi2]));
			vi0 = ((y + 0) % (segmentsY + 1)) * segmentsX + ((x + 0) % (segmentsX));
			vi1 = ((y + 0) % (segmentsY + 1)) * segmentsX + ((x + 1) % (segmentsX));
			vi2 = ((y + 1) % (segmentsY + 1)) * segmentsX + ((x + 1) % (segmentsX));
			triangles.push_back(Triangle(vertices[vi0], vertices[vi1], vertices[vi2]));
		}
	}
	convexMesh = ConvexMesh(&triangles);
}

float Capsule::getRadius() const
{
	return radius;
}

void Capsule::setRadius(float radius)
{
	this->radius = radius;
}

Vector3& Capsule::getA()
{
	return a;
}

Vector3& Capsule::getB()
{
	return b;
}

void Capsule::fromBoundingVolume(BoundingVolume* original)
{
	// check for same type of original
	if (dynamic_cast< Capsule* >(original) != nullptr == false) {
		return;
	}
	auto capsule = dynamic_cast< Capsule* >(original);
	a.set(capsule->a);
	b.set(capsule->b);
	center.set(capsule->center);
	radius = capsule->radius;
	convexMesh = capsule->convexMesh;
	convexMeshA.set(capsule->a);
	convexMeshB.set(capsule->b);
	convexMeshRadius = capsule->radius;
}

void Capsule::fromBoundingVolumeWithTransformations(BoundingVolume* original, const Transformations& transformations)
{
	// check for same type of original
	if (dynamic_cast< Capsule* >(original) != nullptr == false) {
		return;
	}
	auto capsule = dynamic_cast< Capsule* >(original);
	auto& transformationsMatrix = transformations.getTransformationsMatrix();
	transformationsMatrix.multiply(capsule->a, a);
	transformationsMatrix.multiply(capsule->b, b);
	// note:
	//	capsule radius can only be scaled the same on all axes
	//	thats why its enough to only take x axis to determine scaling
	Vector3 side;
	side.set(capsule->a).addX(capsule->radius);
	transformationsMatrix.multiply(side, side);
	radius = side.sub(a).computeLength();
	convexMeshA.set(a);
	convexMeshB.set(b);
	convexMeshRadius = radius;
	convexMesh.fromBoundingVolumeWithTransformations(&capsule->convexMesh, transformations);
	update();
}

void Capsule::update()
{
	if (convexMeshA.equals(a) == false ||
		convexMeshB.equals(b) == false ||
		Math::abs(convexMeshRadius - radius) > Math::EPSILON) {
		convexMeshA.set(a);
		convexMeshB.set(b);
		convexMeshRadius = radius;
		createConvexMesh();
	}
	Vector3 baSub;
	baSub.set(b).sub(a);
	auto baSubLength = baSub.computeLength();
	center.set(a).add(baSub.normalize().scale(baSubLength * 0.5f));
	sphereRadius = baSubLength / 2.0f + radius;
}

void Capsule::computeClosestPointOnBoundingVolume(const Vector3& point, Vector3& closestPoint) const
{
	Vector3 baSub;
	Vector3 paSub;
	baSub.set(b).sub(a);
	auto baSubLength = baSub.computeLength();
	if (baSubLength > 0.0f) {
		baSub.normalize();
		auto t = 0.0f;
		t = Vector3::computeDotProduct(paSub.set(point).sub(a), baSub) / baSubLength;
		if (t < 0.0f)
			t = 0.0f;

		if (t > 1.0f)
			t = 1.0f;

		closestPoint.set(a).add(baSub.scale(t * baSubLength));
	} else {
		closestPoint.set(a);
	}
}

bool Capsule::containsPoint(const Vector3& point) const
{
	Vector3 cpCvsP;
	computeClosestPointOnBoundingVolume(point, cpCvsP);
	auto distance = cpCvsP.sub(point).computeLength();
	return distance <= radius;
}

bool Capsule::doesCollideWith(BoundingVolume* bv2, const Vector3& movement, CollisionResponse* collision)
{
	if (dynamic_cast< BoundingBox* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< BoundingBox* >(bv2), movement, collision);
	} else if (dynamic_cast< OrientedBoundingBox* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< OrientedBoundingBox* >(bv2), movement, collision);
	} else if (dynamic_cast< Sphere* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< Sphere* >(bv2), movement, collision);
	} else if (dynamic_cast< Capsule* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< Capsule* >(bv2), movement, collision);
	} else if (dynamic_cast< Triangle* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< Triangle* >(bv2), movement, collision);
	} else if (dynamic_cast< ConvexMesh* >(bv2) != nullptr) {
		return CollisionDetection::doCollide(this, dynamic_cast< ConvexMesh* >(bv2), movement, collision);
	} else {
		return false;
	}
}

float Capsule::computeDimensionOnAxis(const Vector3& axis) const
{
	Vector3 baSub;
	return Math::abs(Vector3::computeDotProduct(baSub.set(b).sub(a), axis)) + (radius * 2.0f);
}

BoundingVolume* Capsule::clone() const
{
	return new Capsule(a, b, radius);
}

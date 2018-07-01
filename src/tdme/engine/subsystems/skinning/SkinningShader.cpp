#include <tdme/engine/subsystems/skinning/SkinningShader.h>

#include <string>

#include <tdme/engine/Engine.h>
#include <tdme/engine/model/Group.h>
#include <tdme/engine/model/Joint.h>
#include <tdme/engine/model/JointWeight.h>
#include <tdme/engine/model/Model.h>
#include <tdme/engine/model/Skinning.h>
#include <tdme/engine/subsystems/manager/VBOManager.h>
#include <tdme/engine/subsystems/manager/VBOManager_VBOManaged.h>
#include <tdme/engine/subsystems/renderer/GLRenderer.h>
#include <tdme/engine/subsystems/renderer/GL3Renderer.h>
#include <tdme/engine/subsystems/rendering/Object3DGroupMesh.h>
#include <tdme/engine/subsystems/rendering/Object3DGroupVBORenderer.h>
#include <tdme/engine/subsystems/rendering/ObjectBuffer.h>
#include <tdme/utils/ByteBuffer.h>
#include <tdme/utils/Console.h>
#include <tdme/utils/IntBuffer.h>
#include <tdme/utils/FloatBuffer.h>

using std::copy;
using std::begin;
using std::end;
using std::to_string;
using std::string;

using tdme::engine::subsystems::skinning::SkinningShader;

using tdme::engine::Engine;
using tdme::engine::model::Group;
using tdme::engine::model::Joint;
using tdme::engine::model::JointWeight;
using tdme::engine::model::Model;
using tdme::engine::model::Skinning;
using tdme::engine::subsystems::manager::VBOManager;
using tdme::engine::subsystems::manager::VBOManager_VBOManaged;
using tdme::engine::subsystems::renderer::GLRenderer;
using tdme::engine::subsystems::renderer::GL3Renderer;
using tdme::engine::subsystems::rendering::Object3DGroupMesh;
using tdme::engine::subsystems::rendering::Object3DGroupVBORenderer;
using tdme::engine::subsystems::rendering::ObjectBuffer;
using tdme::utils::ByteBuffer;
using tdme::utils::Console;
using tdme::utils::IntBuffer;
using tdme::utils::FloatBuffer;

SkinningShader::SkinningShader(GLRenderer* renderer)
{
	this->renderer = renderer;
	isRunning = false;
	initialized = false;
}

bool SkinningShader::isInitialized()
{
	return initialized;
}

void SkinningShader::initialize()
{
	auto rendererVersion = renderer->getGLVersion();

	// shader
	shaderId = renderer->loadShader(
		renderer->SHADER_COMPUTE_SHADER,
		"shader/" + rendererVersion + "/skinning",
		"skinning.c"
	);
	if (shaderId == 0) return;

	// create, attach and link program
	programId = renderer->createProgram();
	renderer->attachShaderToProgram(programId, shaderId);

	// link program
	if (renderer->linkProgram(programId) == false) return;

	// get uniforms
	uniformSkinningJointsTransformationsMatrices = renderer->getProgramUniformLocation(programId, "skinningJointsTransformationsMatrices");
	if (uniformSkinningJointsTransformationsMatrices == -1) return;

	uniformSkinningCount = renderer->getProgramUniformLocation(programId, "skinningCount");
	if (uniformSkinningCount == -1) return;

	//
	initialized = true;
}

void SkinningShader::useProgram()
{
	isRunning = true;
	renderer->useProgram(programId);
}

void SkinningShader::computeSkinning(Object3DGroupMesh* object3DGroupMesh)
{
	if (isRunning == false) {
		useProgram();
	}

	// Hack: fix me
	auto gl3Renderer = dynamic_cast<GL3Renderer*>(renderer);

	// check if vbo already created
	auto vboBaseIds = object3DGroupMesh->object3DGroupVBORenderer->vboBaseIds;
	if (vboBaseIds == nullptr) return;

	ModelSkinningCache* modelSkinningCacheCached;
	auto group = object3DGroupMesh->group;
	auto& vertices = *group->getVertices();
	auto id = group->getModel()->getId() + "." + group->getId();
	auto cacheIt = cache.find(id);
	if (cacheIt == cache.end()) {
		ModelSkinningCache modelSkinningCache;

		auto skinning = group->getSkinning();
		auto& jointsWeights = *skinning->getVerticesJointsWeights();
		auto& weights = *skinning->getWeights();

		// vbo
		auto vboManaged = Engine::getVBOManager()->addVBO(id + ".vbo", 5);
		modelSkinningCache.vboIds = vboManaged->getVBOGlIds();

		// vertices
		{
			object3DGroupMesh->setupVerticesBuffer(renderer, (*modelSkinningCache.vboIds)[0]);
		}

		// normals
		{
			object3DGroupMesh->setupNormalsBuffer(renderer, (*modelSkinningCache.vboIds)[1]);
		}

		{
			// vertices joints
			auto ibVerticesJoints = ObjectBuffer::getByteBuffer(vertices.size() * 1 * sizeof(int))->asIntBuffer();
			for (int groupVertexIndex = 0; groupVertexIndex < vertices.size(); groupVertexIndex++) {
				int vertexJoints = jointsWeights[groupVertexIndex].size();
				// put number of joints
				ibVerticesJoints.put(vertexJoints);
			}
			gl3Renderer->uploadSkinningBufferObject((*modelSkinningCache.vboIds)[2], ibVerticesJoints.getPosition() * sizeof(int), &ibVerticesJoints);
		}

		{
			// vertices joints indices
			auto ibVerticesVertexJointsIdxs = ObjectBuffer::getByteBuffer(vertices.size() * 4 * sizeof(float))->asIntBuffer();
			for (int groupVertexIndex = 0; groupVertexIndex < vertices.size(); groupVertexIndex++) {
				int vertexJoints = jointsWeights[groupVertexIndex].size();
				// vertex joint idx 1..4
				for (int i = 0; i < 4; i++) {
					ibVerticesVertexJointsIdxs.put(vertexJoints > i?jointsWeights[groupVertexIndex][i].getJointIndex():-1);
				}
			}
			gl3Renderer->uploadSkinningBufferObject((*modelSkinningCache.vboIds)[3], ibVerticesVertexJointsIdxs.getPosition() * sizeof(int), &ibVerticesVertexJointsIdxs);
		}

		{
			// vertices joints weights
			auto fbVerticesVertexJointsWeights = ObjectBuffer::getByteBuffer(vertices.size() * 4 * sizeof(float))->asFloatBuffer();
			for (int groupVertexIndex = 0; groupVertexIndex < vertices.size(); groupVertexIndex++) {
				int vertexJoints = jointsWeights[groupVertexIndex].size();
				// vertex joint weight 1..4
				for (int i = 0; i < 4; i++) {
					fbVerticesVertexJointsWeights.put(static_cast<float>(vertexJoints > i?weights[jointsWeights[groupVertexIndex][i].getWeightIndex()]:0.0f));
				}
			}
			gl3Renderer->uploadSkinningBufferObject((*modelSkinningCache.vboIds)[4], fbVerticesVertexJointsWeights.getPosition() * sizeof(float), &fbVerticesVertexJointsWeights);
		}

		// add to cache
		cache[id] = modelSkinningCache;
		modelSkinningCacheCached = &cache[id];
	} else {
		modelSkinningCacheCached = &cacheIt->second;
	}

	// bind
	gl3Renderer->bindSkinningVerticesBufferObject((*modelSkinningCacheCached->vboIds)[0]);
	gl3Renderer->bindSkinningNormalsBufferObject((*modelSkinningCacheCached->vboIds)[1]);
	gl3Renderer->bindSkinningVertexJointsBufferObject((*modelSkinningCacheCached->vboIds)[2]);
	gl3Renderer->bindSkinningVertexJointIdxsBufferObject((*modelSkinningCacheCached->vboIds)[3]);
	gl3Renderer->bindSkinningVertexJointWeightsBufferObject((*modelSkinningCacheCached->vboIds)[4]);

	// bind output / result buffers
	gl3Renderer->bindSkinningVerticesResultBufferObject((*vboBaseIds)[1]);
	gl3Renderer->bindSkinningNormalsResultBufferObject((*vboBaseIds)[2]);

	// upload matrices
	{
		auto skinning = group->getSkinning();
		auto skinningJoints = skinning->getJoints();
		auto fbMatrices = ObjectBuffer::getByteBuffer(60 * 16 * sizeof(float))->asFloatBuffer();
		for (auto& joint: *skinningJoints) {
			fbMatrices.put(object3DGroupMesh->skinningMatrices->find(joint.getGroupId())->second->getArray());
		}
		gl3Renderer->setProgramUniformFloatMatrices4x4(
			uniformSkinningJointsTransformationsMatrices,
			fbMatrices.getPosition() / 16,
			&fbMatrices
		);
	}

	// skinning count
	gl3Renderer->setProgramUniformInteger(uniformSkinningCount, vertices.size());

	// do it so
	gl3Renderer->dispatchCompute((int)Math::ceil(vertices.size() / 16.0f), 1, 1);
}

void SkinningShader::unUseProgram()
{
	isRunning = false;
}

void SkinningShader::reset() {
	// TODO
}

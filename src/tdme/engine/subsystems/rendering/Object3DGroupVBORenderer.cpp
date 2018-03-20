#include <tdme/engine/subsystems/rendering/Object3DGroupVBORenderer.h>

#include <vector>

#include <tdme/utils/FloatBuffer.h>
#include <tdme/utils/ShortBuffer.h>
#include <tdme/engine/Engine.h>
#include <tdme/engine/model/Group.h>
#include <tdme/engine/model/TextureCoordinate.h>
#include <tdme/engine/subsystems/manager/VBOManager_VBOManaged.h>
#include <tdme/engine/subsystems/manager/VBOManager.h>
#include <tdme/engine/subsystems/rendering/Object3DGroup.h>
#include <tdme/engine/subsystems/rendering/Object3DGroupMesh.h>
#include <tdme/engine/subsystems/rendering/Object3DVBORenderer.h>
#include <tdme/engine/subsystems/renderer/GLRenderer.h>
#include <tdme/math/Vector3.h>

using std::vector;

using tdme::engine::subsystems::rendering::Object3DGroupVBORenderer;
using tdme::utils::FloatBuffer;
using tdme::utils::ShortBuffer;
using tdme::engine::Engine;
using tdme::engine::model::Group;
using tdme::engine::model::TextureCoordinate;
using tdme::engine::subsystems::manager::VBOManager_VBOManaged;
using tdme::engine::subsystems::manager::VBOManager;
using tdme::engine::subsystems::rendering::Object3DGroup;
using tdme::engine::subsystems::rendering::Object3DGroupMesh;
using tdme::engine::subsystems::rendering::Object3DVBORenderer;
using tdme::engine::subsystems::renderer::GLRenderer;
using tdme::math::Vector3;

Object3DGroupVBORenderer::Object3DGroupVBORenderer(Object3DGroup* object3DGroup) 
{
	this->object3DGroup = object3DGroup;
	this->vboBaseIds = nullptr;
	this->vboTangentBitangentIds = nullptr;
}

void Object3DGroupVBORenderer::preRender(Object3DVBORenderer* object3DVBORenderer)
{
	auto meshUploaded = true;
	// initialize if not yet done
	if (vboBaseIds == nullptr) {
		auto vboManaged = Engine::getInstance()->getVBOManager()->addVBO(object3DGroup->id, object3DGroup->mesh->group->getTextureCoordinates() != nullptr ? 4 : 3);
		vboBaseIds = vboManaged->getVBOGlIds();
		meshUploaded = vboManaged->isUploaded();
	}
	// initialize tangents, bitangents
	if (object3DVBORenderer->renderer->isNormalMappingAvailable() && object3DGroup->mesh->group->getTangents()->size() > 0 && object3DGroup->mesh->group->getBitangents()->size() > 0 && vboTangentBitangentIds == nullptr) {
		auto vboManaged = Engine::getInstance()->getVBOManager()->addVBO(object3DGroup->id + ".tangentbitangent", 2);
		vboTangentBitangentIds = vboManaged->getVBOGlIds();
	}
	// check if to upload new mesh
	if (object3DGroup->mesh->hasRecreatedBuffers() == true || meshUploaded == false) {
		if (meshUploaded == false) {
			// upload indices
			object3DGroup->mesh->setupVertexIndicesBuffer(object3DVBORenderer->renderer, (*vboBaseIds)[0]);
			// upload texture coordinates
			if (object3DGroup->mesh->group->getTextureCoordinates() != nullptr) {
				object3DGroup->mesh->setupTextureCoordinatesBuffer(object3DVBORenderer->renderer, (*vboBaseIds)[3]);
			}
		}
		// upload vertices
		object3DGroup->mesh->setupVerticesBuffer(object3DVBORenderer->renderer, (*vboBaseIds)[1]);
		// upload normals
		object3DGroup->mesh->setupNormalsBuffer(object3DVBORenderer->renderer, (*vboBaseIds)[2]);
		// tangents, bitangents
		if (vboTangentBitangentIds != nullptr) {
			object3DGroup->mesh->setupTangentsBuffer(object3DVBORenderer->renderer, (*vboTangentBitangentIds)[0]);
			object3DGroup->mesh->setupBitangentsBuffer(object3DVBORenderer->renderer, (*vboTangentBitangentIds)[1]);
		}
	}
}

void Object3DGroupVBORenderer::dispose()
{
	if (vboBaseIds != nullptr) {
		Engine::getInstance()->getVBOManager()->removeVBO(object3DGroup->id);
		vboBaseIds = nullptr;
	}
	if (vboSkinningIds != nullptr) {
		Engine::getInstance()->getVBOManager()->removeVBO(object3DGroup->id + ".tangentbitangent");
		vboSkinningIds = nullptr;
	}
}
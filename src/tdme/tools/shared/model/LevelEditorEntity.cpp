#include <tdme/tools/shared/model/LevelEditorEntity.h>

#include <vector>

#include <tdme/engine/model/Model.h>
#include <tdme/math/Vector3.h>
#include <tdme/tools/shared/model/LevelEditorEntity_EntityType.h>
#include <tdme/tools/shared/model/LevelEditorEntityBoundingVolume.h>
#include <tdme/tools/shared/model/LevelEditorEntityModel.h>
#include <tdme/tools/shared/model/LevelEditorEntityParticleSystem.h>
#include <tdme/tools/shared/model/LevelEditorEntityPhysics.h>

using std::vector;

using tdme::tools::shared::model::LevelEditorEntity;
using tdme::engine::model::Model;
using tdme::math::Vector3;
using tdme::tools::shared::model::LevelEditorEntity_EntityType;
using tdme::tools::shared::model::LevelEditorEntityBoundingVolume;
using tdme::tools::shared::model::LevelEditorEntityModel;
using tdme::tools::shared::model::LevelEditorEntityParticleSystem;

constexpr int32_t LevelEditorEntity::ID_NONE;

LevelEditorEntity::LevelEditorEntity(int32_t id, LevelEditorEntity_EntityType* entityType, const string& name, const string& description, const string& entityFileName, const string& fileName, const string& thumbnail, Model* model, const Vector3& pivot)
{
	this->id = id;
	this->type = entityType;
	this->name = name;
	this->description = description;
	this->entityFileName = entityFileName;
	this->fileName = fileName;
	this->thumbnail = thumbnail;
	this->model = model;
	this->pivot.set(pivot);
	this->lodLevel2 = nullptr;
	this->lodLevel3 = nullptr;
	this->physics = nullptr;
	this->particleSystem = nullptr;
	this->modelSettings = nullptr;
	if (this->type == LevelEditorEntity_EntityType::PARTICLESYSTEM) {
		this->particleSystem = new LevelEditorEntityParticleSystem();
		this->physics = new LevelEditorEntityPhysics();
	} else
	if (this->type == LevelEditorEntity_EntityType::MODEL) {
		this->modelSettings = new LevelEditorEntityModel(this);
		this->physics = new LevelEditorEntityPhysics();
	}
	renderGroups = false;
	shaderId = "default";
	dynamicShadowing = true;
}

LevelEditorEntity::~LevelEditorEntity() {
	if (model != nullptr) delete model;
	if (lodLevel2 != nullptr) delete lodLevel2;
	if (lodLevel3 != nullptr) delete lodLevel3;
	if (physics != nullptr) delete physics;
	if (particleSystem != nullptr) delete particleSystem;
	if (modelSettings != nullptr) delete modelSettings;
	for (auto i = 0; i < boundingVolumes.size(); i++) {
		delete boundingVolumes[i];
	}
}

bool LevelEditorEntity::addBoundingVolume(int32_t idx, LevelEditorEntityBoundingVolume* levelEditorEntityBoundingVolume)
{
	if (idx < 0)
		return false;

	if (idx > boundingVolumes.size())
		return false;

	if (idx == boundingVolumes.size()) {
		boundingVolumes.push_back(levelEditorEntityBoundingVolume);
	}
	return false;
}

void LevelEditorEntity::setDefaultBoundingVolumes()
{
	for (auto i = boundingVolumes.size(); i < 8; i++) {
		auto bv = new LevelEditorEntityBoundingVolume(i, this);
		addBoundingVolume(i, bv);
	}
}

void LevelEditorEntity::setLODLevel2(LevelEditorEntityLODLevel* lodLevel) {
	if (this->type != LevelEditorEntity_EntityType::MODEL) return;
	if (lodLevel2 != nullptr) delete lodLevel2;
	lodLevel2 = lodLevel;
}

void LevelEditorEntity::setLODLevel3(LevelEditorEntityLODLevel* lodLevel) {
	if (this->type != LevelEditorEntity_EntityType::MODEL) return;
	if (lodLevel3 != nullptr) delete lodLevel3;
	lodLevel3 = lodLevel;
}

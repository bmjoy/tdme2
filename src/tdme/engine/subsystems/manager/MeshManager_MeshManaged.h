#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/engine/subsystems/manager/fwd-tdme.h>
#include <tdme/engine/subsystems/rendering/fwd-tdme.h>

using std::string;

using tdme::engine::subsystems::manager::MeshManager;
using tdme::engine::subsystems::rendering::Object3DGroupMesh;

/** 
 * Managed mesh entity
 * @author Andreas Drewke
 */
class tdme::engine::subsystems::manager::MeshManager_MeshManaged
{
	friend class MeshManager;

private:
	string id;
	Object3DGroupMesh* mesh { nullptr };
	int32_t referenceCounter { 0 };
private:

	/** 
	 * Protected constructor
	 * @param id id
	 * @param mesh mesh
	 */
	MeshManager_MeshManaged(const string& id, Object3DGroupMesh* mesh);

	/**
	 * @return mesh id
	 */
	inline const string& getId() {
		return id;
	}

	/** 
	 * @return object 3d group mesh
	 */
	inline Object3DGroupMesh* getMesh() {
		return mesh;
	}

	/** 
	 * @return reference counter
	 */
	inline int32_t getReferenceCounter() {
		return referenceCounter;
	}

	/** 
	 * decrement reference counter
	 * @return if reference counter = 0
	 */
	inline bool decrementReferenceCounter() {
		referenceCounter--;
		return referenceCounter == 0;
	}

	/** 
	 * increment reference counter
	 */
	inline void incrementReferenceCounter() {
		referenceCounter++;
	}

};

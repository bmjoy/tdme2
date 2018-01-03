#pragma once

#include <array>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/utils/fwd-tdme.h>
#include <tdme/utils/FloatBuffer.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/model/TextureCoordinate.h>
#include <tdme/engine/subsystems/object/fwd-tdme.h>
#include <tdme/engine/subsystems/renderer/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>

using std::array;
using std::vector;

using tdme::utils::ByteBuffer;
using tdme::utils::FloatBuffer;
using tdme::engine::model::TextureCoordinate;
using tdme::engine::subsystems::renderer::GLRenderer;
using tdme::math::Vector3;

/** 
 * Batch VBO renderer
 * @author andreas.drewke
 * @version $Id$
 */
class tdme::engine::subsystems::object::BatchVBORendererTriangles final
{
	friend class TransparentRenderFacesGroup;

private:
	static constexpr int32_t VERTEX_COUNT { 1024 * 3 };
	GLRenderer* renderer {  };
	vector<int32_t>* vboIds {  };
	int32_t id {  };
	bool acquired {  };
	int32_t vertices {  };
	ByteBuffer* fbVerticesByteBuffer;
	FloatBuffer fbVertices {  };
	ByteBuffer* fbNormalsByteBuffer {  };
	FloatBuffer fbNormals {  };
	ByteBuffer* fbTextureCoordinatesByteBuffer {  };
	FloatBuffer fbTextureCoordinates {  };
	static array<float, 2> TEXTURECOORDINATE_NONE;

	/**
	 * Clears this batch vbo renderer
	 */
	void clear();

	/**
	 * Render
	 */
	void render();

	/**
	 * Adds a vertex to this transparent render faces group
	 * @param vertex
	 * @param normal
	 * @param texture coordinate
	 * @return success
	 */
	inline bool addVertex(const Vector3& vertex, const Vector3& normal, TextureCoordinate* textureCoordinate) {
		// check if full
		if (vertices == VERTEX_COUNT)
			return false;
		// otherwise
		fbVertices.put(vertex.getArray());
		fbNormals.put(normal.getArray());
		if (textureCoordinate != nullptr) {
			fbTextureCoordinates.put(textureCoordinate->getArray());
		} else {
			fbTextureCoordinates.put(&TEXTURECOORDINATE_NONE);
		}
		vertices++;
		return true;
	}

public:

	/** 
	 * @return acquired
	 */
	bool isAcquired();

	/** 
	 * Acquire
	 */
	bool acquire();

	/** 
	 * Release
	 */
	void release();

	/** 
	 * Init
	 */
	void initialize();

	/** 
	 * Dispose
	 */
	void dispose();
	/**
	 * Public constructor
	 * @param renderer
	 * @param id
	 */
	BatchVBORendererTriangles(GLRenderer* renderer, int32_t id);

	/**
	 * Destructor
	 */
	~BatchVBORendererTriangles();
};

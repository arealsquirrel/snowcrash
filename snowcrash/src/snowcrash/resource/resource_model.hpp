#ifndef SC_RESOURCE_MODEL_HPP
#define SC_RESOURCE_MODEL_HPP

#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/core/object.hpp"
#include "snowcrash/math/vector.hpp"
#include "snowcrash/resource/resource.hpp"
#include <functional>
#include <snowcrash/core/core.hpp>

namespace SC {

class ResourceModel : public Resource {
SC_OBJECT(ResourceModel, Resource)

public:
	struct Vertex {
		Vector3 position;
		Vector3 color;
		Vector2 texCoord;

		bool operator ==(const Vertex &vertex) const { 
			return (vertex.texCoord == texCoord && 
					color == vertex.color &&
					position == vertex.position);
		}
	};

public:
	ResourceModel(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID=UUID(0));

	~ResourceModel();

public:
	void load(ResourceLoadError *error) override;
	void load_metadata(ResourceLoadError *error) override;
	void save() override;
	void unload() override;

public:
	const ArrayList<Vertex> &get_verticies() const { return m_verticies; }
	const ArrayList<u32> &get_indices() const { return m_indices; }

private:
	ArrayList<Vertex> m_verticies;
	ArrayList<u32> m_indices;
};

}

namespace std {
    template<> struct hash<SC::ResourceModel::Vertex> {
        size_t operator()(SC::ResourceModel::Vertex const& vertex) const {
            return ((SC::hash_vector(vertex.color.x, vertex.color.y, vertex.color.z) ^ 
					(SC::hash_vector(vertex.position.x, vertex.position.y, vertex.position.z) << 1) >> 1) ^ 
					(SC::hash_vector(vertex.texCoord.x, vertex.texCoord.y) << 1) << 1);
        }
    };
}

#endif

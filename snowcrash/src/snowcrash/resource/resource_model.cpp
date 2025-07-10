
#include "resource_model.hpp"

#include <external/tiny_obj_loader.hpp>

namespace SC {


ResourceModel::ResourceModel(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID) 
	: Resource(allocator, engine, path, name, groupID),
	  m_verticies(m_allocator),
	  m_indices(m_allocator) {}

ResourceModel::~ResourceModel() {

}

void ResourceModel::load(ResourceLoadError *error) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, m_path.c_str());

	std::unordered_map<Vertex, u32> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.position = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = {1.0f, 1.0f, 1.0f};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(m_verticies.get_count());
				m_verticies.add(vertex);
			}

			m_indices.add(uniqueVertices[vertex]);
		}
	}
}

void ResourceModel::load_metadata(ResourceLoadError *error) {
	if(m_loadState == ResourceLoadState_Partialy) return;

	ResourceModel::load_metadata(error);
}

void ResourceModel::save() {

}

void ResourceModel::unload() {
	m_verticies.reset();
	m_indices.reset();
}

}


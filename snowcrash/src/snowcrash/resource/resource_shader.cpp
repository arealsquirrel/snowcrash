
#include "resource_shader.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/resource/resource.hpp"

namespace SC {

ResourceShader::ResourceShader(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {

}

ResourceShader::~ResourceShader() {
	if(m_loadState == ResourceLoadState_FullyLoaded) {
		m_allocator->free_mem(m_shader, m_size);
	}
}

void ResourceShader::load_metadata(ResourceLoadError *error) {
	if(m_loadState == ResourceLoadState_Partialy) return;
	SC_CORE_INFO("Shader has no metadata {}", m_path.c_str());
	ResourceShader::load_metadata(error);
}

void ResourceShader::load(ResourceLoadError *error) {
	Pair<char*,u32> bfile = read_file_bytes(error, m_path.c_str());

	m_shader = bfile.first;
	m_size = bfile.second;

	Resource::load(error);
}

void ResourceShader::save() {
	SC_ASSERT(false, "not implemented");
}

void ResourceShader::unload() {
	m_allocator->free_mem(m_shader, m_size);
}

}


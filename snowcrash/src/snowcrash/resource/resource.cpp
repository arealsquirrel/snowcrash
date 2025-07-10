
#include "resource.hpp"
#include <fstream>
#include <snowcrash/core/logging.hpp>
#include <nlohmann/json.hpp>

namespace SC {

Resource::Resource(Allocator *allocator, Engine *engine,
			String path, String name, UUID groupID)
	: Object(engine, allocator), m_path(path), m_name(name), m_groupID(groupID) {

}


Resource::~Resource() = default;

void Resource::load(ResourceLoadError *error) {
	SC_ASSERT(m_loadState != ResourceLoadState_FullyLoaded, 
			"resource already loaded. If you are remaking the resource, please call unload first");

	if(*error != ResourceLoadError_None) {
		SC_CORE_WARN("Resource::load returning with error {}", 
				load_error_to_string(*error));

		m_loadState = ResourceLoadState_Unloaded;
	}

	m_loadState = ResourceLoadState_FullyLoaded;
}

void Resource::load_metadata(ResourceLoadError *error) {
	if(*error != ResourceLoadError_None) {
		SC_CORE_WARN("Resource::load_metadata returning with error {}", 
				load_error_to_string(*error));

		m_loadState = ResourceLoadState_Unloaded;
	}

	m_loadState = ResourceLoadState_Partialy;
}

void Resource::save() {
	SC_ASSERT(false, "not implemented yet");
}

void Resource::unload() {
	SC_ASSERT(m_loadState != ResourceLoadState_Unloaded, "resource already unloaded");
}

Pair<char*, u32> Resource::read_file_strings(ResourceLoadError *error, const char *path) {
	std::ifstream file(m_path.c_str());

	if(file.fail()) {
		*error = ResourceLoadError_PathDoesNotExist;
		SC_CORE_WARN("read_file_bytes ifstream failed on file {}", m_path.c_str());
		return {nullptr, 0};
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0);

	char *buffer = m_allocator->allocate_mem(size); 
	file.read(buffer, size);

	return Pair<char*, u32>(buffer, size);
}

Pair<char*, u32> Resource::read_file_bytes(ResourceLoadError *error, const char *path) {
	std::ifstream file(m_path.c_str(), std::ios::ate | std::ios::binary);
	
	if(file.fail()) {
		*error = ResourceLoadError_PathDoesNotExist;
		SC_CORE_WARN("read_file_bytes ifstream failed on file {}", m_path.c_str());
		return {nullptr, 0};
	}
	
	u32 fileSize = (size_t) file.tellg();
	file.seekg(0);
	
	char *buffer = m_allocator->allocate_mem(fileSize);
	file.read(buffer, fileSize);
	return Pair<char*, u32>(buffer, fileSize);
}


nlohmann::json Resource::read_file_json(ResourceLoadError *error, const char *path) {
	std::ifstream file(m_path.c_str());
	nlohmann::json j;

	try {
		j = nlohmann::json::parse(file); 
	} catch (nlohmann::json::exception exception) {
		SC_CORE_ERROR("nlohmann::json excpetion thrown while reading file {}\n \
				{}",m_path.c_str(), exception.what());
		*error = ResourceLoadError_ApiError;
	}
	
	return j;
}

}


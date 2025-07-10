
#include "resource_image.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/resource/resource.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>

#include <external/stb_image.hpp>

namespace SC {

ResourceImage::ResourceImage(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID) 
	: Resource(allocator, engine, path, name, groupID),
	  m_metadata({m_allocator,""}, ColorFormat_R8, {0,0}, ArrayList<Frame>(m_allocator)) {

}

ResourceImage::~ResourceImage() = default;

void ResourceImage::load(ResourceLoadError *error) {
	load_metadata(error);

	int x;
	int y;

	SC_CORE_INFO("loading");
	m_image = stbi_load(m_metadata.image.c_str(),
			  	&x, &y,
			 	(int*)(&m_channels), 0);

	SC_CORE_INFO("wow");

	Resource::load(error);
}

void ResourceImage::load_metadata(ResourceLoadError *error) {
	if(m_loadState == ResourceLoadState_Partialy) return;

	if(std::filesystem::path(m_path.c_str()).extension() == ".json") {
		SC_CORE_INFO("image points to metadata, loading that");
		SC_ASSERT(false, "not implemented yet");
		nlohmann::json jsonMeta = read_file_json(error, m_path.c_str());
	} else {
		SC_CORE_INFO("metadata");
		int n; // ??????
		u32 ok = stbi_info(m_path.c_str(),
				&m_metadata.size.x, &m_metadata.size.y,
				&n);

		// make some default metadata
		m_metadata.image = m_path.c_str();
		m_metadata.frames.add({
			Rect{0, 0, (u32)m_metadata.size.x, (u32)m_metadata.size.y},
			false,
			false,
			Rect{0, 0, (u32)m_metadata.size.x, (u32)m_metadata.size.y},
			0
		});
	}

	Resource::load_metadata(error);
}

void ResourceImage::save() {
	Resource::save();
}

void ResourceImage::unload() {
	stbi_image_free(m_image);

	Resource::unload();
}

}


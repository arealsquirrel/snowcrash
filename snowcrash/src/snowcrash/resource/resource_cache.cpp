
#include "resource_cache.hpp"

namespace SC {

ResourceCache::ResourceCache(Engine *engine, Allocator *allocator)
	: Object(engine, allocator),
	m_resources(allocator),
	m_stringHashToIndex(allocator, 10) {}
// , ,
ResourceCache::~ResourceCache() {
	m_resources.for_each([&](Resource *r, u32 index){
		if(r != nullptr) {
			r->unload();
			m_allocator->free(r);
		}
	});
}

}


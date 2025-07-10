#ifndef SC_RESOURCE_CACHE_HPP
#define SC_RESOURCE_CACHE_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/container/hashmap.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/core/object.hpp"
#include "snowcrash/engine/engine.hpp"
#include "snowcrash/resource/resource.hpp"
#include <functional>
#include <snowcrash/core/core.hpp>

namespace SC {

/*
 * stores the resources and allows you to map them
 * to paths or strings
 */
class ResourceCache : public Object {
SC_OBJECT(ResourceCache, Object)

public:
	ResourceCache(Engine *engine, Allocator *allocator);
	~ResourceCache();

	/*
	 * fully loads and initilizes all the resources
	 */
	template<typename T>
	T &load_resource(const char *path, const char *name) {
		T *resource = m_allocator->allocate<T>(1, m_allocator, m_engine, String(m_allocator, path), String(m_allocator, name), 0);
		Resource *r = dynamic_cast<Resource*>(resource);
		Resource::ResourceLoadError err = Resource::ResourceLoadError_None;
		r->load(&err);

		if(err != Resource::ResourceLoadError_None) {
			SC_CORE_ERROR("resource cache got error {}\n{}",
					resource->load_error_to_string(err), path);
		}
		
		u32 index = m_resources.get_count();
		m_resources.add(r);
		m_stringHashToIndex.add({String::hash(name), index});

		return *resource;
	}

	/*
	 * loads just the metadata and puts the resource state
	 * in partialy loaded
	 */
	template<typename T, typename ...Args>
	T &load_resource_metadata(const char *path, const char *name) {
		/*
		T *resource = m_allocator->allocate<T>(1, m_allocator, m_engine, String(m_allocator, path), String(m_allocator, name), 0);
		Resource *r = dynamic_cast<Resource*>(resource);
		Resource::ResourceLoadError err = Resource::ResourceLoadError_None;
		r->load_metadata(&err);

		if(err != Resource::ResourceLoadError_None) {
			SC_CORE_ERROR("resource cache got error {}\n{}", resource->load_error_to_string(err), path);
		}

		u32 index = m_resources.get_count();
		m_resources.add(r);
		*/
		// m_stringToIndex.add({r->get_name(), index});
	}

	template<typename T>
	T *get_resource(const char *name) {
		auto p = m_stringHashToIndex.find_entry(String::hash(name));
		return dynamic_cast<T*>(m_resources.get(p.second));
	}

private:
	ArrayList<Resource*> m_resources;
	HashMap<u32, u32> m_stringHashToIndex;
};

};

#endif

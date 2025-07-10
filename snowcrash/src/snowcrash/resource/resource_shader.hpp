#ifndef SC_RESOURCE_SHADER_HPP
#define SC_RESOURCE_SHADER_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/core/object.hpp"
#include "snowcrash/engine/engine.hpp"
#include "snowcrash/resource/resource.hpp"
#include <snowcrash/core/core.hpp>

namespace SC {

/*
 * this class holds a compiled shader. ANY
 * SHADER THAT HAS BEEN COMPILED.
 */
class ResourceShader : public Resource {
SC_OBJECT(ResourceShader, Resource)

public:
	ResourceShader(Allocator *allocator, Engine *engine,
			 String path, String name, UUID groupID=UUID(0));
	~ResourceShader();

public:
	void load_metadata(ResourceLoadError *error) override;
	void load(ResourceLoadError *error) override;
	void save() override;
	void unload() override;

public:
	const char *get_shader() const { return m_shader; }
	u32 get_size() const { return m_size; }

private:
	char *m_shader;
	u32 m_size;
};

}

#endif

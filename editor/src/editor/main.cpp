
#include "snowcrash/core/core.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/core/uuid.hpp"
#include "snowcrash/engine/engine.hpp"
#include "snowcrash/resource/resource_cache.hpp"
#include "snowcrash/resource/resource_image.hpp"
#include "snowcrash/resource/resource_model.hpp"
#include "snowcrash/resource/resource_shader.hpp"

using namespace snowcrash;

int main(int args, char **argv) {
	Log::get_core_log()->add_sink<LogSinkColorCout>();
	Log::get_core_log()->add_sink<LogSinkFile>("core.log");

	SC_CORE_TRACE("tracing {}", 2);
	SC_CORE_WARN("dumbass {}", UUID());

	Engine engine({SC_KILLOBYTE(400),
				   SC_KILLOBYTE(200),
				   SC_KILLOBYTE(16)});

	{
		ResourceCache cache(&engine, engine.get_allocator_persistent());
		
		cache.load_resource<ResourceImage>(
				"../snowcrash/resources/SCPrimitaves/textures/viking_room.png",
				"viking_room");
		
		ResourceImage *image = cache.get_resource<ResourceImage>("viking_room");
		SC_CORE_INFO("image {} {}", 
		  		image->get_meta_data().size.x, image->get_meta_data().size.y);
		
		cache.load_resource<ResourceShader>(
				"../snowcrash/resources/SCPrimitaves/shaders/bin/triangle.vert.spv",
				"triangle.vert.spv");
		
		
		SC_CORE_INFO("vertex size {}",
				cache.get_resource<ResourceShader>("triangle.vert.spv")->get_size());

		
		cache.load_resource<ResourceModel>(
				"../snowcrash/resources/SCPrimitaves/models/monkey.obj",
				"monkey.obj");
		
		
		// SC_CORE_INFO("model vertex count {}",
		// 	cache.get_resource<ResourceModel>("monkey.obj")->get_verticies().get_size());


		// engine.dump_mem();
	}
	
	SC_CORE_TRACE("closing");
	engine.dump_mem();
}


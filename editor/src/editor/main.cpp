
#include "snowcrash/core/core.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/core/uuid.hpp"
#include "snowcrash/engine/engine.hpp"

using namespace snowcrash;

int main(int args, char **argv) {
	Log::get_core_log()->add_sink<LogSinkColorCout>();
	Log::get_core_log()->add_sink<LogSinkFile>("core.log");

	SC_CORE_TRACE("tracing {}", 2);
	SC_CORE_WARN("dumbass {}", UUID());

	Engine engine({SC_KILLOBYTE(200),
				   SC_KILLOBYTE(40),
				   SC_KILLOBYTE(16)});

	engine.dump_mem();
}


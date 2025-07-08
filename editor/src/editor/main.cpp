
#include "snowcrash/container/allocator.hpp"
#include "snowcrash/core/core.hpp"
#include "snowcrash/core/logging.hpp"
#include <iostream>

using namespace snowcrash;

int main(int args, char **argv) {
	Log::get_core_log()->add_sink<LogSinkColorCout>();
	Log::get_core_log()->add_sink<LogSinkFile>("core.log");

	SC_CORE_TRACE("tracing {}", 2);
	SC_CORE_WARN("dumbass");
}


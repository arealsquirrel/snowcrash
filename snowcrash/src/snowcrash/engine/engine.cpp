
#include "engine.hpp"
#include "snowcrash/core/core.hpp"
#include "snowcrash/core/logging.hpp"

namespace SC {

Engine::Engine(EngineSpecs specs) 
	: m_engineMemory(specs.persistentMemorySize+specs.frameMemorySize+specs.dynamicMemorySize),
	  m_persistentMemory(m_engineMemory.partition(specs.persistentMemorySize)),
	  m_persistentMemoryAllocator(&m_persistentMemory),
	  m_dynamicMemory(m_engineMemory.partition(specs.dynamicMemorySize, specs.persistentMemorySize)),
	  m_dynamicMemoryAllocator(&m_dynamicMemory),
	  m_frameMemory(m_engineMemory.partition(specs.frameMemorySize, specs.persistentMemorySize + specs.dynamicMemorySize)),
	  m_frameAllocator(&m_frameMemory) {

	SC_CORE_INFO("Snowcrash Engine created\n \
			version {}\n \
			engine mem {}\n \
			persistent {}\n \
			dynamic {}\n \
			frame {}", SC_VERSION, specs.dynamicMemorySize + specs.frameMemorySize + specs.persistentMemorySize,
					 specs.persistentMemorySize,
					 specs.frameMemorySize,
					 specs.dynamicMemorySize);
}

Engine::~Engine() = default;

}


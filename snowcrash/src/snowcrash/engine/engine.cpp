
#include "engine.hpp"
#include "snowcrash/container/allocator.hpp"
#include "snowcrash/core/core.hpp"
#include "snowcrash/core/logging.hpp"

namespace SC {

Engine::Engine(EngineSpecs specs) 
	: Object(this),
	  m_engineMemory(specs.persistentMemorySize+specs.frameMemorySize+specs.dynamicMemorySize),
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

void Engine::dump_mem() const {
	SC_CORE_TRACE("------- ENGINE PERSISTENT MEMORY -------");
	SC_CORE_TRACE("mem block starts at {} size {}",
	 		(void*)m_persistentMemory.get_memory(), m_persistentMemory.get_size());
	FreeListAllocator::Header *node = m_persistentMemoryAllocator.get_start();
	while(node != nullptr) {
		SC_CORE_TRACE("\theader at {} size {}", (void*)node, node->size);
		node = node->next;
	}

	SC_CORE_TRACE("------- ENGINE DYNAMIC MEMORY -------");
	SC_CORE_TRACE("mem block starts at {} size {}",
			(void*)m_dynamicMemory.get_memory(), m_dynamicMemory.get_size());
	node = m_dynamicMemoryAllocator.get_start();
	while(node != nullptr) {
		SC_CORE_TRACE("\theader at {} size {}", (void*)node, node->size);
		node = node->next;
	}

	SC_CORE_TRACE("------- ENGINE FRAME MEMORY -------");
	SC_CORE_TRACE("mem block starts at {} size {}",
			(void*)m_frameMemory.get_memory(), m_frameMemory.get_size());
}

}


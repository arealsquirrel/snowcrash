
#ifndef SC_ENGINE_HPP
#define SC_ENGINE_HPP

#include "snowcrash/container/allocator.hpp"
#include <snowcrash/core/core.hpp>

namespace SC {

struct EngineSpecs {
	u32 persistentMemorySize;
	u32 dynamicMemorySize;
	u32 frameMemorySize;
};

/*
 * is a monolith class that holds all that is needed to run the engine
 */
class Engine {
public:
	Engine(EngineSpecs specs);
	~Engine();

private:
	MemoryBlock m_engineMemory;

	
	MemoryBlock m_persistentMemory;
	FreeListAllocator m_persistentMemoryAllocator;

	MemoryBlock m_dynamicMemory;
	FreeListAllocator m_dynamicMemoryAllocator;

	MemoryBlock m_frameMemory;
	StackAllocator m_frameAllocator;
};

}

#endif


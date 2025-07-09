
#ifndef SC_ENGINE_HPP
#define SC_ENGINE_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/core/object.hpp"
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
class Engine : public Object{
SC_OBJECT(Engine, Object)

public:
	Engine(EngineSpecs specs);
	~Engine();

public:
	FreeListAllocator *get_allocator_persistent() { return &m_persistentMemoryAllocator; }
	FreeListAllocator *get_allocator_dynamic() { return &m_dynamicMemoryAllocator; }
	StackAllocator *get_allocator_frame() { return &m_frameAllocator; }

	void dump_mem() const;

private:
	/* ------------ MEMORY ------------ */ 
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


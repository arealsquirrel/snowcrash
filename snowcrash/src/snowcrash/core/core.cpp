
#include "core.hpp"
#include <cstdlib>

namespace SC {

static_assert(sizeof(u8) == 1, "u8 is not the right size");
static_assert(sizeof(i8) == 1, "i8 is not the right size");
static_assert(sizeof(u16) == 2, "u16 is not the right size");
static_assert(sizeof(i16) == 2, "i16 is not the right size");
static_assert(sizeof(u32) == 4, "u32 is not the right size");
static_assert(sizeof(i32) == 4, "i32 is not the right size");
static_assert(sizeof(u64) == 8, "u64 is not the right size");
static_assert(sizeof(i64) == 8, "i64 is not the right size");

MemoryBlock::MemoryBlock(u32 size)
	: m_size(size), m_partition(false) {
	
	m_memory = reinterpret_cast<char*>(malloc(m_size));
}

MemoryBlock::MemoryBlock(char *block, u32 size)
	: m_memory(block), m_size(size), m_partition(true) {}

MemoryBlock::~MemoryBlock() {
	if(m_partition == false) { 
		free(m_memory);
	}
}

MemoryBlock MemoryBlock::partition(u32 size, u32 offset) {
	return MemoryBlock(m_memory+offset, size);	
}

}


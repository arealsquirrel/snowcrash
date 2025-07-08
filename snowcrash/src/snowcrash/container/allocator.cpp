
#include "allocator.hpp"
#include "snowcrash/core/core.hpp"

#include <new>

namespace SC {

Allocator::Allocator(const MemoryBlock *block)
	: m_block(block) {}

Allocator::~Allocator() {

}

StackAllocator::StackAllocator(const MemoryBlock *block)
	: Allocator(block) {
	
	m_stackTop = m_block->get_memory();
}

StackAllocator::~StackAllocator() = default;

char *StackAllocator::allocate_mem(u32 size) {
	char *mem = m_stackTop;
	m_stackTop += size;
	return mem;
}

void StackAllocator::free_mem(char *mem, u32 size) {
	m_stackTop -= size;
}

u32 StackAllocator::get_allocated_mem() const {
	return m_stackTop - m_block->get_memory();
}

FreeListAllocator::FreeListAllocator(const MemoryBlock *block)
	: Allocator(block) {
	
	// make a linked list in the memory
	m_start = new (block->get_memory()) Header{
		block->get_size() - (u32)sizeof(Header),
		true,
		nullptr,
		nullptr
	};

	m_end = new (block->get_memory()+block->get_size()-sizeof(Header)) Header{
		0,
		false,
		nullptr,
		m_start
	};

	m_start->next = m_end;
}

FreeListAllocator::~FreeListAllocator() = default;

char *FreeListAllocator::allocate_mem(u32 size) {
	Header *best = nullptr;

	Header *current = m_start;
	while(current != m_end) {

		if(current->free) {
			if(best == nullptr) {
				best = current;
			}

			if(current->size == size) {
				return reinterpret_cast<char*>(current) + sizeof(Header);
			} else if(current->size > size && current->size < best->size) {
				best = current;
			}
		}

		current = current->next;
	}

	Header *split = new (
			SC_MEM_POINTER(best) + size + sizeof(Header)
		) Header {best->size - size - (u32)sizeof(Header), true, m_end, best};

	best->free = false;
	best->size = size;
	best->next = split;

	return SC_MEM_POINTER(best) + sizeof(Header);
}

void FreeListAllocator::free_mem(char *mem, u32 size) {
	Header *memHeader = reinterpret_cast<Header*>(
			SC_MEM_POINTER(mem) - sizeof(Header));

	memHeader->free = true;
}

}


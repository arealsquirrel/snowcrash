
#include "allocator.hpp"
#include "snowcrash/core/core.hpp"

#include <iostream>
#include <iterator>
#include <new>

namespace SC {

Allocator::Allocator(const MemoryBlock *block)
	: m_block(block) {}

Allocator::~Allocator() {

}

char *Allocator::allocate_mem(u32 size) {
	return new char[size];
}

void Allocator::free_mem(char *mem, u32 size) {
	delete[] mem;
}

u32 Allocator::get_allocated_mem() const {
	return 0;
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
	u32 totalSize = size + sizeof(Header);
	while(current != m_end) {
		if(current->free == false || current->size < totalSize) {
			current = current->next;
			continue;
		}

		if(current->size == totalSize) {
			return reinterpret_cast<char*>(current) + sizeof(Header);
		} 

		if(best == nullptr || current->size < best->size) {
			best = current;
		}

		current = current->next;
	}

	Header *split = new (
			SC_MEM_POINTER(best+1) + size
		) Header {best->size - size - (u32)(sizeof(Header)), true, best->next, best};

	best->free = false;
	best->size = size;
	best->next->prev = split;
	best->next = split;

	return SC_MEM_POINTER(best) + sizeof(Header);
}

void FreeListAllocator::free_mem(char *mem, u32 size) {
	Header *memHeader = reinterpret_cast<Header*>(
			SC_MEM_POINTER(mem) - sizeof(Header));

	memHeader->free = true;
	merge_freed(memHeader);
}

void FreeListAllocator::merge_freed(Header *header) {
	if(header != m_end && header->next->free) {
		header->size += header->next->size + sizeof(Header);
		header->next = header->next->next;
		header->next->prev = header;
	}

	if(header != m_start && header->prev->free) {
		Header *h = header->prev;
		h->size += header->size + sizeof(Header);
		h->next = header->next;
		h->next->prev = h;
	}
}

}



#ifndef SC_ALLOCATOR_HPP
#define SC_ALLOCATOR_HPP

#include <snowcrash/core/core.hpp>
#include <utility>
#include <new>

namespace SC {

class Allocator {
public:
	/*
	 * give it the block thats its gonna manage
	 */
	Allocator(const MemoryBlock *block);
	~Allocator();

public:
	virtual char *allocate_mem(u32 size);
	virtual void free_mem(char *mem, u32 size);

	virtual u32 get_allocated_mem() const;

public:
	/*
	 * calls the constructor
	 */
	template<class T, typename ...Args>
	T *allocate(u32 size=1, Args &&...args) {
		T *mem = reinterpret_cast<T*>(allocate_mem(sizeof(T) * size));
		for(int i = 0; i < size; i++)
			new (mem+i) T(std::forward<Args>(args)...);
		return mem;
	}

	/*
	 * calls the destructor
	 */
	template<class T>
	void free(T *mem, u32 size=1) {
		for(int i = 0; i < size; i++)
		 	(mem+i)->~T();
		this->free_mem(SC_MEM_POINTER(mem), size*sizeof(T));
	}

public:
	const MemoryBlock *get_block() const { return m_block; }

protected:
	const MemoryBlock *m_block;
};

class StackAllocator : public Allocator {
public:
	StackAllocator(const MemoryBlock *block);
	~StackAllocator();

	char *allocate_mem(u32 size) override;

	/*
	 * we dont really care about the mem field
	 * with this one
	 */
	void free_mem(char *mem, u32 size) override;

	u32 get_allocated_mem() const override;

	void reset() { m_stackTop = m_block->get_memory(); }

private:
	char *m_stackTop;
};

/*
 * uses a doubly linked list to find usable blocks
 * of memory and easy traversal of the memory space
 */
class FreeListAllocator : public Allocator {
public:
	struct alignas(8) Header {
		u32 size;
		bool free;

		Header *next;
		Header *prev;
	};

public:
	FreeListAllocator(const MemoryBlock *block);
	~FreeListAllocator();

	char *allocate_mem(u32 size) override;

	/*
	 * we dont really care about the mem field
	 * with this one
	 */
	void free_mem(char *mem, u32 size) override;

	/*
	 * @warning not implemented yet
	 */
	u32 get_allocated_mem() const override { return 0; }

	Header *get_start() const { return m_start; }
	Header *get_end() const { return m_end; }

private:
	void merge_freed(Header *header);

private:
	Header *m_start;
	Header *m_end;
};

}

#endif



#ifndef SC_ARRAYLIST_HPP
#define SC_ARRAYLIST_HPP

#include "snowcrash/container/allocator.hpp"
#include <alloca.h>
#include <functional>
#include <iostream>
#include <snowcrash/core/core.hpp>

namespace SC {

template<typename T>
class ArrayList { 
public:
	ArrayList(Allocator *allocator)
		: m_allocator(allocator) {
		
		m_size = 0;
	}

	ArrayList(Allocator *allocator, u32 size)
		: m_allocator(allocator) {
		
		resize(size);
	}
	
	~ArrayList() {
		reset();
	}

	/*
	 * inherits the allocator
	 */
	ArrayList(const ArrayList &arraylist)
		: m_allocator(arraylist.m_allocator) {

		if(arraylist.empty()) return;

		resize(arraylist.m_size);

		for(int i = 0; i < arraylist.m_count; i++) {
			add(arraylist.get_array()[i]);
		}
	}

public:
	T *get_array() const { return m_buffer; }

	T &add(const T &element) {
		if(m_count >= m_size) {
			resize(m_size+1);
		}

		T *e = new (m_buffer+m_count) T(element);
		m_count++;
		return *e;
	}

	/*
	 * removes the top item by just decrementing count
	 * so the allocated size stays the same
	 */
	void remove() {
		m_count--;
	}
	
	/*
	 * uses the copy constructor when copying the arrays
	 * size must be greater than the current size
	 */
	void resize(u32 size) {
		if(m_buffer == nullptr) {
			m_buffer = reinterpret_cast<T*>(
					m_allocator->allocate_mem(sizeof(T) * size));
			m_size = size;
			return;
		}

		T *temp = reinterpret_cast<T*>(m_allocator->allocate_mem(sizeof(T) * size));
		for(int i = 0; i < m_count; i++) {
			new (temp+i) T(m_buffer[i]);

			m_buffer[i].~T();
		}

		m_allocator->free_mem(SC_MEM_POINTER(m_buffer), get_byte_size());

		m_buffer = temp;
		m_size = size;
	}

	void reset() {
		for(int i = 0; i < m_count; i++) {
			m_buffer[i].~T();
		}

		m_allocator->free_mem(SC_MEM_POINTER(m_buffer), get_byte_size());
	}

	u32 get_byte_size() const { return sizeof(T) * m_size; }
	u32 get_size() const { return m_size; }
	u32 get_count() const { return m_count; }
	bool empty() const { return (m_buffer == nullptr); }

public:
	T &get(u32 index) { return m_buffer[index]; }
	const T &get(u32 index) const { return m_buffer[index]; }

	T &operator[](u32 index) { return m_buffer[index]; }
	const T &operator[](u32 index) const { return m_buffer[index]; }

public:
	void for_each(std::function<void(T &element, u32 index)> func) {
		for(int i = 0; i < m_count; i++) {
			func(m_buffer[i], i);
		}
	}

private:
	T *m_buffer {nullptr};

	Allocator *m_allocator;
	u32 m_size {0};
	u32 m_count {0};
};

}

#endif



#include "string.hpp"
#include "snowcrash/container/allocator.hpp"
#include <algorithm>
#include <cstring>

namespace SC {

String::String(Allocator *allocator)
	: m_allocator(allocator) {

	m_size = 0;
}

String::String(Allocator *allocator, u32 size) 
	: m_allocator(allocator) {
	
	resize(size);
}

String::String(Allocator *allocator, const char *string)
	: m_allocator(allocator) {

	*this = string;
}

String::String(const String &string)
	: m_allocator(string.m_allocator) {

	*this = string.m_buffer;
}

String::~String() {
	m_allocator->free_mem(m_buffer, m_size+1);
}

void String::resize(u32 size) {
	if(m_buffer == nullptr) {
		m_buffer = m_allocator->allocate_mem(size+1);
		m_size = size;
		return;
	}

	char *temp = m_allocator->allocate_mem(size+1);
	memcpy(temp, m_buffer, std::min(size, m_size));

	m_allocator->free_mem(m_buffer, m_size+1);

	m_buffer = temp;
	m_size = size;
}

u32 String::hash(const char *str) {
	u32 b = 1;
	u32 len = strlen(str);
	for (int i = 0; i < len; i++) {
		b += i * 123123 + len;
	}

	return b;
}

}


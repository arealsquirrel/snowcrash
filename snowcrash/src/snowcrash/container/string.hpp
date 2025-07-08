
#ifndef SC_STRING_HPP
#define SC_STRING_HPP

#include "snowcrash/container/allocator.hpp"
#include <cstring>
#include <snowcrash/core/core.hpp>

namespace SC {

struct String {
public:
	String(Allocator *allocator);
	String(Allocator *allocator, const char *string);
	String(const String &string);
	~String();

public:
	char *c_str() const { return m_buffer; }
	u32 get_size() const { return m_size; }

	u32 compare(const char *str) const { return strcmp(m_buffer, str); }
	bool equals(const char *str) const { return (compare(str) == 0); }

	void operator =(const char *str) {
		u32 len = strlen(str);
		resize(len);
		strcpy(m_buffer, str);
	}

private:
	/*
	 * implicitly adds one to the size in order
	 * to reserve the null byte
	 */
	void resize(u32 size);

private:
	char *m_buffer {nullptr};
	u32 m_size;
	Allocator *m_allocator;
};

}

#endif


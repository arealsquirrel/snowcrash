
#ifndef SC_STRING_HPP
#define SC_STRING_HPP

#include "snowcrash/container/allocator.hpp"
#include <cstring>
#include <functional>
#include <snowcrash/core/core.hpp>

namespace SC {

struct String {
public:
	String(Allocator *allocator);
	String(Allocator *allocator, const char *string);
	String(Allocator *allocator, u32 size);
	String(const String &string);
	~String();

public:
	char *c_str() const { return m_buffer; }
	u32 get_size() const { return m_size; }

	u32 compare(const char *str) const { return strcmp(m_buffer, str); }
	bool equals(const char *str) const { return (compare(str) == 0); }

	// String substring(size_t pos, size_t len) const;
	// String substring(int length) const { return substring(0, length); }

	void operator =(const char *str) {
		u32 len = strlen(str);
		resize(len);
		strcpy(m_buffer, str);
	}

	static u32 hash(const char *str);

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

namespace std {
	template<>
	struct hash<SC::String> {
		std::size_t operator()(const SC::String &string) {
			return std::hash<const char *>()(string.c_str());
		}
	};
}

#endif


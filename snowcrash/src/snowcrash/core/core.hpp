
#ifndef SC_CORE_HPP
#define SC_CORE_HPP

#define SC snowcrash

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define SC_WINDOWS

#ifdef _WIN64
	#define SC_BIT_64
	#define SC_WINDOWS_64
#elif
	#define SC_BIT_32
	#define SC_WINDOWS_32
#endif

#elif __APPLE__
	#define SC_APPLE
	#error "AH HELL NAW"
#elif __linux__
	#define SC_LINUX
#endif

#ifdef __GNUC__
#if __x86_64 || ppc64 
#define SC_BIT_64
#else
#define SC_BIT_32
#endif
#endif

#define DEBUG
	
#ifdef DEBUG
#define SC_DEBUG
#define SC_LOGGING
#define SC_ASSERT
#else

#endif

#define SC_MEM_POINTER(var) reinterpret_cast<char*>(var)
#define SC_VERSION "1.0.0"

namespace SC {

// type sizes as defined by this
// https://en.cppreference.com/w/cpp/language/types.html
// 32-bit = ILP32
// 64-bit = LLP64 for windows, LP64 for unix and linux

using u8 = unsigned char;
using i8 = char;

using u16 = unsigned short int;
using i16 = short int;

using u32 = unsigned int;
using i32 = int;

using u64 = unsigned long long int;
using i64 = long long int;

/*
 * standard way of describing a block of malloced memory
 */
struct MemoryBlock {
public:
	/*
	 * uses malloc to make some new memory.
	 * this will free the memory at the end of its lifetime
	 */
	MemoryBlock(u32 size);
	
	/*
	 * used to partition memory blocks
	 * will not free at the end of its lifetime
	 */
	MemoryBlock(char *block, u32 size);
	
	~MemoryBlock();

public:
	char *get_memory() const { return m_memory; }
	u32 get_size() const { return m_size; }
	MemoryBlock partition(u32 size, u32 offset=0);

private:
	char *m_memory;
	u32 m_size;
	bool m_partition;
};

}

#endif



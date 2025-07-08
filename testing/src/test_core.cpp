#include "../external/doctest.h"
#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/array.hpp"
#include "snowcrash/core/core.hpp"
#include <iostream>

using namespace snowcrash;

TEST_CASE("allocators") {
	MemoryBlock block(1204);

	MemoryBlock stackBlock = block.partition(256);

	SUBCASE("StackAllocator") {
		StackAllocator sa(&stackBlock);
		u16 *a = sa.allocate<u16>();
		*a = 20;
		sa.allocate<u8>();
		sa.allocate<u64>();
		REQUIRE(sa.get_allocated_mem() == 11);
	}

	MemoryBlock flBlock = block.partition(600);

	SUBCASE("FreeListAllocator") {
		FreeListAllocator fa(&flBlock);
		u32 *arr = fa.allocate<u32>(5);
		arr[0] = 100;
		arr[4] = 234234;
		u8  *arr1 = fa.allocate<u8>(3);
		arr1[0] = 123;
		arr1[2] = 122;
		u16 *arr2 = fa.allocate<u16>(2);

		fa.free_mem(SC_MEM_POINTER(arr2), sizeof(u16) * 2);
		
		u8 *arr3 = fa.allocate<u8>(3);

		REQUIRE(arr[0] == 100);
		REQUIRE(arr[4] == 234234);
		REQUIRE(arr1[2] == 122);
	}
}

class foo {
public:
	foo() {
		std::cout << "constructor!" << std::endl;
	}

	foo(int _a)
		: a(_a) {
		
		
	}

	~foo() {
		std::cout << "destructor!" << std::endl;
	}

public:
	int a {1};
};

TEST_CASE("array") {
	MemoryBlock block(1204);
	FreeListAllocator fa(&block);

	Array<foo, 4> fooArr(&fa);

	fooArr.for_each([](foo &foo, int index){
		foo.a += 2;
	});

	fooArr.for_each([](foo &foo, int index){
		REQUIRE(foo.a == 3);
	});

	REQUIRE(fooArr.get(0).a == 3);
}

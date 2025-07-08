#include "../external/doctest.h"
#include "snowcrash/container/allocator.hpp"
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

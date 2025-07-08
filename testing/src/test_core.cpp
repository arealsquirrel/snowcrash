#include "../external/doctest.h"
#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/array.hpp"
#include "snowcrash/container/arraylist.hpp"
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
	foo() = default;

	foo(const foo &f) = default;

	foo(int _a)
		: a(_a) {}

	~foo() = default;

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

TEST_CASE("arraylist") {
	MemoryBlock block(1204);
	FreeListAllocator fa(&block);

	ArrayList<foo> array(&fa);
	array.add(foo(5));
	array.add(foo(2));
	array.add(foo(3));

	ArrayList<foo> newArray(array);
	newArray.add(foo(1));
	newArray.remove();
	newArray.add(foo(3));
	newArray[0].a = 4;

	REQUIRE(array[0].a == 5);
	REQUIRE(array.get_count() == 3);
	REQUIRE(array.get_size() == 3);
	REQUIRE(newArray.get_size() == 4);
	REQUIRE(newArray[0].a == 4);
	REQUIRE(newArray.get_count() == 4);

	newArray.for_each([](foo &f, u32 i){ std::cout << f.a << std::endl; });
}


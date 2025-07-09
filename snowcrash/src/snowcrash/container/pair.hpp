
#ifndef SC_PAIR_HPP
#define SC_PAIR_HPP

#include <snowcrash/core/core.hpp>
#include <utility>

namespace SC {

template<class First, class Second>
struct Pair {
public:
	Pair() = default;

	Pair(const First &_first, const Second &_second)
		: first(_first), second(_second) {}

	Pair(const Pair &pair)
		: first(pair.first), second(pair.second) {}

	~Pair() = default;

public:
	First first;
	Second second;
};

template<class First, class Second>
void swap_pair(const Pair<First, Second> &a, const Pair<First, Second> &b) {
	// std::swap(a.first, b.first);
	// std::swap(a.second, b.second);
}

}

#endif


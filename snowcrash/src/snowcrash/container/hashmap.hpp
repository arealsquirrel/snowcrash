
#ifndef SC_HASHMAP_HPP
#define SC_HASHMAP_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/container/pair.hpp"
#include <functional>
#include <limits>
#include <snowcrash/core/core.hpp>
#include <utility>

#include <snowcrash/container/result.hpp>

namespace SC {

/*
 * the Key type needs to support std::hash
 * you can do the [const Key] to get values
 */
template<class Key, class Value>
class HashMap {
public:
	static constexpr u32 c_skipIndex = 5;
	static constexpr u32 c_max_psl = std::numeric_limits<u32>::max();

public:
	struct Node {
		Pair<const Key, Value> pair;
		u32 psl;
	};

public:
	/*
	 * the elements field is how many elements
	 * you estimate will be in here. Its gonna
	 * use the load index to get the actual size
	 */
	HashMap(Allocator *allocator, u32 elements)
		: m_allocator(allocator) {
	
		m_buckets = elements / 0.75f;
		
		std::cout << "mem " << m_buckets * c_skipIndex * sizeof(Node) << std::endl;

		m_nodes = reinterpret_cast<Node*>(
				m_allocator->allocate_mem(m_buckets * c_skipIndex * sizeof(Node)));

		for (int i = 0; i < m_buckets*c_skipIndex; i++) {
			m_nodes[i].psl = c_max_psl;
		}
	}

	~HashMap() {
		for_each([](auto &pair){
			pair.first.~Key();
			pair.second.~Value();
		});

		m_allocator->free_mem(SC_MEM_POINTER(m_nodes), get_size() * sizeof(Node));
	}

public:
	Pair<const Key, Value> &add(const Pair<const Key, Value> &entry) {
		auto &tempEntry = entry;
		u32 hash = m_hash(entry.first);
		u32 entryIndex = (hash % m_buckets) * c_skipIndex;
		u32 vpsl = 0;

		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(vpsl > m_nodes[entryIndex].psl) {
				swap_pair(tempEntry, m_nodes[entryIndex].pair);
				std::swap(vpsl, m_nodes[entryIndex].psl);
			}

			entryIndex = (entryIndex+1) % get_size();
			vpsl++;
		}

		Node *n = new (m_nodes+entryIndex) Node{entry, vpsl};
		return n->pair;
	}

	void remove(const Key &key) {
		u32 hash = m_hash(key);
		u32 entryIndex = find_entry_index(key);
		
		m_nodes[entryIndex].pair.first.~Key();
		m_nodes[entryIndex].pair.second.~Value();
		m_nodes[entryIndex].psl = c_max_psl;

		entryIndex = (entryIndex+1) % get_size();
		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(m_nodes[entryIndex].psl > 0) {
				m_nodes[entryIndex].psl -= 1;
				swap_pair(m_nodes[entryIndex].pair,
						  m_nodes[entryIndex-1].pair);
				std::swap(m_nodes[entryIndex].psl, m_nodes[entryIndex-1].psl);
			} else {
				return;
			}

			entryIndex = (entryIndex+1) % get_size();
		}
	}

	inline Pair<const Key, Value> &find_entry(const Key &key) {
		return m_nodes[find_entry_index(key)].pair;
	}

	void for_each(std::function<void( Pair<const Key, Value>& )> func) {
		for(int i = 0; i < get_size(); i++) {
			if(m_nodes[i].psl != c_max_psl) {
				func(m_nodes[i].pair);
			}
		}
	}

public:
	inline u32 get_count() const { return m_count; }
	inline u32 get_size() const { return m_buckets * c_skipIndex; }
	inline float load_factor() { return ((float)m_count / (m_buckets * c_skipIndex)); }

	bool has_key(const Key &key) const { return (find_entry_index(key) != c_max_psl); }

	Node *get_array() const { return m_nodes; }

	Pair<const Key, Value> &operator[](const Key &key) { return find_entry(key); }

private:
	inline u32 find_entry_index(const Key &key) const {
		u32 hash = m_hash(key);
		u32 entryIndex = (hash % m_buckets) * c_skipIndex;
		u32 vpsl = 0;

		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(vpsl > m_nodes[entryIndex].psl) {
				 return c_max_psl;
			}

			if(hash == m_hash(m_nodes[entryIndex].pair.first)) {
				return entryIndex;
			}

			entryIndex = (entryIndex+1) % get_size();
			vpsl++;
		}

		return c_max_psl;
	}

private:
	Allocator *m_allocator;

	u32 m_buckets;
	u32 m_count {0};

	std::hash<Key> m_hash;

	Node *m_nodes;
};

}

#endif


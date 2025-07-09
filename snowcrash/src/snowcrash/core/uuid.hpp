
#ifndef SC_UUID_HPP
#define SC_UUID_HPP

#include <snowcrash/core/core.hpp>

namespace SC {

struct UUID {
public:
	UUID();
	UUID(const UUID &id);
	UUID(u64 id);
	~UUID();

	operator u64() const { return uuid; }
	
	bool operator ==(const UUID &uuid) { return (uuid.uuid == uuid); }

public:
	u64 uuid;
};

// inline constexpr UUID nullUUID(1);

}

namespace std {
	template<>
	struct hash<SC::UUID> {
		std::size_t operator()(const SC::UUID &uuid) {
			return uuid.uuid;
		}
	};

	template <>
	struct formatter<SC::UUID> : formatter<string> {
	  	auto format(SC::UUID p, format_context& ctx) const {
	    	return formatter<string>::format(
	      		std::format("{}", p.uuid), ctx);
  		}
	};
}

#endif


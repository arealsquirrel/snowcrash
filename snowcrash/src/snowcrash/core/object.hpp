
#ifndef SC_OBJECT_HPP
#define SC_OBJECT_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/core/logging.hpp"
#include "snowcrash/core/uuid.hpp"
// #include "snowcrash/engine/engine.hpp"
#include <functional>
#include <snowcrash/core/core.hpp>

namespace SC {

class Engine;

struct TypeInfo {
	const TypeInfo *parent;
	char name[256];
	UUID typeID;
};

class Object {
public:
	explicit Object(Engine *engine, Allocator *allocator=nullptr);
	~Object();

public:
	virtual const TypeInfo *get_typeinfo() const { return get_static_typeinfo(); }
	virtual UUID get_type_UUID() const { return get_static_typeinfo()->typeID; }
	static const TypeInfo *get_static_typeinfo() { static TypeInfo info {nullptr, "Object", UUID()}; return &info; }

public:
	UUID get_instance_id() const { return m_instanceID; }
	const Allocator *get_allocator() const { return m_allocator; }

	/*
	 * T must inherit from object
	 */
	template<typename T>
	bool instance_of() const {
		const TypeInfo *info = get_typeinfo();
		while(info != nullptr) {
			if(info->typeID == T::get_static_typeinfo().typeID) {
				return true;
			}

			info = info->parent;
		}

		return false;
	}

	template<typename T>
	inline T *cast() {
		SC_ASSERT(instance_of<T>(), "this object {} is not an instance of T", *this);
		return dynamic_cast<T*>(this);
	}

	template<typename T>
	inline const T *cast() const {
		SC_ASSERT(instance_of<T>(), "this object {} is not an instance of T", *this);
		return dynamic_cast<T*>(this);
	}

public:

	/*
	 * adds an interface for you to specify how to 
	 * compare this object to others
	 *
	 * generaly, negative is less than
	 * 			 positabe is more than
	 *
	 * this default one returns 0 of the objects have the same instance ID
	 */
	i8 compare(const Object &object) const;

protected:
	UUID m_instanceID;
	Engine *m_engine;
	Allocator *m_allocator;
};

#define SC_OBJECT(className, parent) public: \
	virtual const TypeInfo *get_typeinfo() const override { return get_static_typeinfo(); } \
	virtual UUID get_type_UUID() const override { return get_static_typeinfo()->typeID; } \
	static const TypeInfo *get_static_typeinfo() { static TypeInfo info {parent::get_static_typeinfo(), #className, UUID()}; return &info; } \

}

namespace std {
	template<>
	struct hash<SC::Object> {
		std::size_t operator()(const SC::Object &object) {
			return object.get_instance_id();
		}
	};

	template <>
	struct formatter<SC::Object> : formatter<string> {
	  	auto format(SC::Object o, format_context& ctx) const {
	    	return formatter<string>::format(
	      		std::format("[{}, iuuid {}]", o.get_typeinfo()->name, o.get_instance_id()), ctx);
  		}
	};
}

#endif


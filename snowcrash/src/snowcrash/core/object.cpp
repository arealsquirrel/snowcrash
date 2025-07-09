
#include "object.hpp"

namespace SC {

Object::Object(Engine *engine, Allocator *allocator)
	: m_engine(engine), m_allocator(allocator) {

}

Object::~Object() = default;

i8 Object::compare(const Object &object) const {
	return (object.get_typeinfo()->typeID == get_static_typeinfo()->typeID) ? 0 : 1;
}

}


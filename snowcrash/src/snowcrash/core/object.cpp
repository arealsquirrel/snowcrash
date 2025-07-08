
#include "object.hpp"

namespace SC {

Object::Object(Engine *engine, Allocator *allocator)
	: m_engine(engine), m_allocator(allocator) {

}

Object::~Object() = default;

}


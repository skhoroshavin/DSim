
#pragma once

#include <stdexcept>
#include <vector>

#include <stdint.h>
#include <stddef.h>

#include "json.hpp"

#ifdef NDEBUG
#define DSIM_ASSERT(condition,message)
#else // NDEBUG
#define DSIM_ASSERT(condition,message) if( !(condition) ) throw std::logic_error(message)
#endif // NDEBUG

namespace DSim {

// Group
typedef uint8_t group_t;

// Index
typedef std::pair<size_t,size_t> swap_t;
enum { INVALID_INDEX = size_t(-1) };

// UUID
class UUID
{
public:
	typedef uint64_t type;

	UUID() = default;
	UUID( const UUID& ) = default;

	inline explicit UUID( type id ) : m_id(id) { }
	inline operator type() const { return m_id; }

private:
	type m_id;
};

// JSON
using json = nlohmann::json;

} // namespace DSim


namespace std {

template<> struct hash<DSim::UUID>
{
	typedef DSim::UUID argument_type;
	typedef size_t     result_type;
	inline size_t operator()( DSim::UUID val ) const { return std::hash<DSim::UUID::type>()(val); }
};

} // namespace std

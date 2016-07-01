
#pragma once

#include <stdexcept>
#include <vector>

#include <stdint.h>
#include <stddef.h>

#include "json.hpp"

#ifdef NDEBUG
#define DSIM_ASSERT(condition,message)
#else // NDEBUG
#define DSIM_ASSERT(condition,message) if( condition ) throw std::logic_error(message)
#endif // NDEBUG

namespace DSim {

// Group
typedef uint8_t group_t;

// Index
typedef size_t index_t;
typedef std::pair<index_t,index_t> swap_t;
enum { INVALID_INDEX = index_t(-1) };

// UUID
typedef uint64_t uuid_t;

// JSON
using json = nlohmann::json;

} // namespace DSim

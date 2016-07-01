
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

using json = nlohmann::json;

typedef uint32_t index_t;
typedef std::pair<index_t,index_t> index_pair_t;

} // namespace DSim


#pragma once

#include <stdexcept>

#ifdef NDEBUG
#define DSIM_ASSERT(condition,message)
#else // NDEBUG
#define DSIM_ASSERT(condition,message) if( condition ) throw std::logic_error(message)
#endif // NDEBUG

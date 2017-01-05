
#include "test_datagen.h"

DSIM_BEGIN_HEADER

struct dsim_test_array
{
    void *data;
    size_t count;
};

struct dsim_test_datagen *dsim_test_array_data( struct dsim_test_context *ctx, struct dsim_test_datagen *elem, size_t min_count );

DSIM_END_HEADER

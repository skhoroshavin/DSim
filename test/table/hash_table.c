
#include <criterion/criterion.h>

#include "table/hash_table.h"

static struct dsim_table_column test_columns[] = {
    dsim_table_column_static_init( sizeof(float), &dsim_default_allocator ),
    dsim_table_column_static_init( sizeof(float), &dsim_default_allocator ),
    dsim_table_column_static_init( sizeof(float), &dsim_default_allocator )
};

static struct dsim_hash_table test_table = dsim_hash_table_static_init(
        count_of(test_columns), test_columns,
        &dsim_default_allocator );

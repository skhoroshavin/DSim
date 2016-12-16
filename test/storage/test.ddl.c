
#include "reflection/ddl_registry.h"
#include "storage/hash_storage.h"
#include "test.ddl.h"

static const char dsim_ddl_test_data[] = {
    8, 0, 0, 0, 0, 0, 0, 0, 94, -3, -1, -1, 64, 2, 0, 0,
    -28, 0, 0, 0, 80, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0,
    4, 0, 0, 0, -120, -3, -1, -1, 48, 0, 0, 0, 32, 0, 0, 0,
    20, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0,
    -24, -3, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
    108, 97, 121, 111, 117, 116, 0, 0, 7, 0, 0, 0, 115, 116, 111, 114,
    97, 103, 101, 0, 1, 0, 0, 0, 4, 0, 0, 0, -8, -3, -1, -1,
    120, 0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 0, 76, 0, 0, 0,
    40, 0, 0, 0, 4, 0, 0, 0, 20, -2, -1, -1, 20, 0, 0, 0,
    4, 0, 0, 0, 4, 0, 0, 0, 118, 101, 99, 52, 0, 0, 0, 0,
    1, 0, 0, 0, 118, 0, 0, 0, 52, -2, -1, -1, 20, 0, 0, 0,
    4, 0, 0, 0, 5, 0, 0, 0, 102, 108, 111, 97, 116, 0, 0, 0,
    1, 0, 0, 0, 102, 0, 0, 0, 84, -2, -1, -1, 20, 0, 0, 0,
    4, 0, 0, 0, 5, 0, 0, 0, 117, 105, 110, 116, 56, 0, 0, 0,
    1, 0, 0, 0, 105, 0, 0, 0, 6, 0, 0, 0, 108, 97, 121, 111,
    117, 116, 0, 0, 3, 0, 0, 0, 28, 1, 0, 0, -40, 0, 0, 0,
    4, 0, 0, 0, 120, -2, -1, -1, -64, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 12, 0, 0, 0, 16, 16, 0, 0, 0, 0, 0, 0,
    -102, -2, -1, -1, 4, 0, 0, 0, 4, 0, 0, 0, 124, 0, 0, 0,
    84, 0, 0, 0, 44, 0, 0, 0, 4, 0, 0, 0, -64, -2, -1, -1,
    24, 0, 0, 0, 8, 0, 0, 0, 12, 0, 0, 0, 5, 0, 0, 0,
    102, 108, 111, 97, 116, 0, 0, 0, 1, 0, 0, 0, 119, 0, 0, 0,
    -28, -2, -1, -1, 24, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0,
    5, 0, 0, 0, 102, 108, 111, 97, 116, 0, 0, 0, 1, 0, 0, 0,
    122, 0, 0, 0, 8, -1, -1, -1, 24, 0, 0, 0, 8, 0, 0, 0,
    4, 0, 0, 0, 5, 0, 0, 0, 102, 108, 111, 97, 116, 0, 0, 0,
    1, 0, 0, 0, 121, 0, 0, 0, 52, -1, -1, -1, 20, 0, 0, 0,
    4, 0, 0, 0, 5, 0, 0, 0, 102, 108, 111, 97, 116, 0, 0, 0,
    1, 0, 0, 0, 120, 0, 0, 0, 4, 0, 0, 0, 118, 101, 99, 52,
    0, 0, 0, 0, 120, -1, -1, -1, 48, 0, 0, 0, 32, 0, 0, 0,
    4, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 8, 0, 0, 0,
    0, 0, 0, 0, -120, -1, -1, -1, 1, 1, 0, 0, 5, 0, 0, 0,
    102, 108, 111, 97, 116, 0, 0, 0, 5, 0, 0, 0, 102, 108, 111, 97,
    116, 0, 0, 0, -72, -1, -1, -1, 40, 0, 0, 0, 24, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0,
    -40, -1, -1, -1, 7, 0, 0, 0, 117, 105, 110, 116, 56, 95, 116, 0,
    5, 0, 0, 0, 117, 105, 110, 116, 56, 0, 0, 0, 4, 0, 0, 0,
    116, 101, 115, 116, 0, 0, 0, 0, 4, 0, 4, 0, 16, 0, 28, 0,
    4, 0, 8, 0, 12, 0, 13, 0, 20, 0, 24, 0, 8, 0, 6, 0,
    4, 0, 5, 0, 8, 0, 12, 0, 4, 0, 8, 0, 10, 0, 16, 0,
    4, 0, 8, 0, 12, 0, 6, 0, 8, 0, 4, 0, 16, 0, 22, 0,
    4, 0, 0, 0, 20, 0, 21, 0, 12, 0, 16, 0, 14, 0, 28, 0,
    4, 0, 8, 0, 12, 0, 20, 0, 24, 0, 12, 0, 20, 0, 4, 0,
    8, 0, 12, 0, 16, 0
};

static struct _ddl_test _ddl;
const struct _ddl_test *const ddl_test = &_ddl;

void dsim_ddl_init_test()
{
    dsim_ddl_register( dsim_ddl_test_data );

    _ddl.type_uint8 = dsim_ddl_type( "uint8" );
    _ddl.type_float = dsim_ddl_type( "float" );
    _ddl.type_vec4 = dsim_ddl_type( "vec4" );
    _ddl.layout_layout = dsim_ddl_layout( "layout" );
    _ddl.storage_storage = dsim_ddl_storage( "storage" );
}

void dsim_ddl_done_test()
{
    dsim_ddl_registry_reset();
}



#include <criterion/criterion.h>

#include "containers/hash.h"

static dsim_uint64_hash hash = dsim_hash_init(&dsim_default_allocator);

/*
 * Empty hash
 */

void init_hash_empty()
{

}

void done_hash_empty()
{
    dsim_uint64_hash_reset( &hash );
}

TestSuite(hash_empty, .init = init_hash_empty, .fini = done_hash_empty );

Test(hash_empty, assert_empty)
{
    cr_assert( hash.keys.data == 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity == 0 );

    cr_assert( hash.data.data == 0 );
    cr_assert( hash.data.count == 0 );
    cr_assert( hash.data.capacity == 0 );
}

Test(hash_empty, reserve)
{
    dsim_uint64_hash_reserve( &hash, 10 );

    cr_assert( hash.keys.data != 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity >= 10 );

    cr_assert( hash.data.data != 0 );
    cr_assert( hash.data.count == 0 );
    cr_assert( hash.data.capacity >= 10 );
}

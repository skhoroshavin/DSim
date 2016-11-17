
#include <criterion/criterion.h>

#include "containers/hash.h"

static struct dsim_hash hash = dsim_hash_static_init(&dsim_default_allocator);

/*
 * Empty hash
 */

static void init_hash_empty()
{

}

static void done_hash_empty()
{
    dsim_hash_reset( &hash );
}

TestSuite(hash_empty, .init = init_hash_empty, .fini = done_hash_empty );

Test(hash_empty, assert_empty)
{
    cr_assert( hash.keys.data == 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity == 0 );

    cr_assert( dsim_hash_find( &hash, 0 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 1 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 5 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 10 ) == DSIM_INVALID_INDEX );
}

Test(hash_empty, reserve)
{
    dsim_hash_reserve( &hash, 10 );

    cr_assert( hash.keys.data != 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity >= 10 );

    cr_assert( dsim_hash_find( &hash, 0 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 1 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 5 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 10 ) == DSIM_INVALID_INDEX );
}

/*
Test(hash_empty, insert_one)
{
    dsim_hash_insert( &hash, 5, 1 );

    cr_assert( hash.keys.data != 0 );
    cr_assert( hash.keys.count == 1 );
    cr_assert( hash.keys.capacity >= 1 );

    cr_assert( dsim_hash_find( &hash, 0 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 1 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 5 )  == 0 );
    cr_assert( dsim_hash_find( &hash, 10 ) == DSIM_INVALID_INDEX );
}
*/

Test(hash_empty, clear)
{
    dsim_hash_clear( &hash );

    cr_assert( hash.keys.data == 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity == 0 );

    cr_assert( dsim_hash_find( &hash, 0 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 1 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 5 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 10 ) == DSIM_INVALID_INDEX );
}

Test(hash_empty, reset)
{
    dsim_hash_reset( &hash );

    cr_assert( hash.keys.data == 0 );
    cr_assert( hash.keys.count == 0 );
    cr_assert( hash.keys.capacity == 0 );

    cr_assert( dsim_hash_find( &hash, 0 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 1 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 5 )  == DSIM_INVALID_INDEX );
    cr_assert( dsim_hash_find( &hash, 10 ) == DSIM_INVALID_INDEX );
}

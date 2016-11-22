/*
#include <criterion/criterion.h>

#include "table/hash_table.h"

static struct dsim_table_column table_columns[] = {
    dsim_table_column_static_init( sizeof(uint64_t), &dsim_default_allocator ),
    dsim_table_column_static_init( sizeof(float),    &dsim_default_allocator ),
    dsim_table_column_static_init( 128,              &dsim_default_allocator )
};

static struct dsim_hash_table hash_table = dsim_hash_table_static_init(
        count_of(table_columns), table_columns,
        &dsim_default_allocator );

static struct dsim_table *const table = &hash_table.table;

static void test_hash_table_scheme()
{
    cr_assert( dsim_table_column_count( table ) == count_of(table_columns) );
    for( uint32_t i = 0; i < dsim_table_column_count( table ); ++i )
        cr_assert( dsim_table_column_size( table, i ) == table_columns[i].elem_size );

    cr_assert( dsim_table_block_count( table) == 1 );
}

inline static int check_index( dsim_table_index ti, uint32_t block, uint32_t index )
{
    return (ti.block == block) && (ti.index == index);
}

static void test_hash_table_empty()
{
    cr_assert( dsim_table_block_size( table, 0 ) == 0 );

    cr_assert( dsim_table_id_data( table, 0 ) == 0 );
    for( uint32_t i = 0; i < dsim_table_column_count( table ); ++i )
        cr_assert( dsim_table_data( table, 0, i ) == 0 );

    cr_assert( check_index( dsim_table_find( table, 0 ),  0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 1 ),  0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 25 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 30 ), 0, DSIM_INVALID_INDEX ) );

    cr_assert( table->log.version == 0 );
    cr_assert( table->log.commands.count == 0 );
}

static void test_hash_table_count( uint32_t count )
{
    cr_assert( dsim_table_block_size( table, 0 ) == count );

    cr_assert( dsim_table_id_data( table, 0 ) != 0 );
    for( uint32_t i = 0; i < dsim_table_column_count( table ); ++i )
        cr_assert( dsim_table_data( table, 0, i ) != 0 );
}
*/
/*
 * Empty table
 */
/*
static void init_hash_table_empty()
{

}

static void done_hash_table_empty()
{
    dsim_table_reset( table );
}

TestSuite(hash_table_empty, .init = init_hash_table_empty, .fini = done_hash_table_empty );

Test(hash_table_empty, assert_empty)
{
    test_hash_table_scheme();
    test_hash_table_empty();
}

Test(hash_table_empty, insert)
{
    dsim_table_insert( table, 100, 4 );

    test_hash_table_scheme();
    test_hash_table_count( 4 );

    for( int i = 0; i < 4; ++i )
        cr_assert( dsim_table_id_data( table, 0 )[i] == 100 + i );

    cr_assert( table->log.commands.count == 1 );
    cr_assert( table->log.commands.data[0].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[0].block == 0 );
    cr_assert( table->log.commands.data[0].start == 100 );
    cr_assert( table->log.commands.data[0].count == 4 );
}

Test(hash_table_empty, reset)
{
    dsim_table_reset( table );

    test_hash_table_scheme();
    test_hash_table_empty();
}
*/
/*
 * Filled table
 */
/*
static const uint64_t test_data_0[10] = { 12, 64, 23, 76, 643, 0, 2, 9, 7234, 36 };
static const float    test_data_1[10] = { 0, -1, 23.2, 0.64, -0.0001, -0, -2634.6, 54, 0, 6024 };
static uint8_t test_data_2[10*128];

static void test_hash_table_data()
{
    cr_assert( memcmp( dsim_table_data( table, 0, 0 ), test_data_0, sizeof(test_data_0) ) == 0 );
    cr_assert( memcmp( dsim_table_data( table, 0, 1 ), test_data_1, sizeof(test_data_1) ) == 0 );
    cr_assert( memcmp( dsim_table_data( table, 0, 2 ), test_data_2, sizeof(test_data_2) ) == 0 );

    cr_assert( check_index( dsim_table_find( table, 0 ),   0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 1 ),   0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 20 ),  0, 0 ) );
    cr_assert( check_index( dsim_table_find( table, 27 ),  0, 7 ) );
    cr_assert( check_index( dsim_table_find( table, 324 ), 0, DSIM_INVALID_INDEX ) );
}

static void init_hash_table_filled()
{
    assert( count_of(test_data_0) == 10 );
    assert( count_of(test_data_1) == 10 );
    assert( sizeof(test_data_2) == 10*128 );
    for( size_t i = 0; i < sizeof(test_data_2); ++i )
        test_data_2[i] = rand() & 0xff;

    dsim_table_insert( table, 20, 10 );
    memcpy( dsim_table_data( table, 0, 0 ), test_data_0, sizeof(test_data_0) );
    memcpy( dsim_table_data( table, 0, 1 ), test_data_1, sizeof(test_data_1) );
    memcpy( dsim_table_data( table, 0, 2 ), test_data_2, sizeof(test_data_2) );
}

static void done_hash_table_filled()
{
    dsim_table_reset( table );
}

TestSuite(hash_table_filled, .init = init_hash_table_filled, .fini = done_hash_table_filled );

Test(hash_table_filled, assert_filled)
{
    test_hash_table_scheme();
    test_hash_table_count( 10 );
    test_hash_table_data();

    cr_assert( table->log.commands.count == 1 );
    cr_assert( table->log.commands.data[0].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[0].block == 0 );
    cr_assert( table->log.commands.data[0].start == 20 );
    cr_assert( table->log.commands.data[0].count == 10 );
}

Test(hash_table_filled, insert_more)
{
    dsim_table_insert( table, 15, 3 );

    test_hash_table_scheme();
    test_hash_table_count( 13 );
    test_hash_table_data();

    cr_assert( check_index( dsim_table_find( table, 15 ), 0, 10 ) );
    cr_assert( check_index( dsim_table_find( table, 17 ), 0, 12 ) );
    cr_assert( check_index( dsim_table_find( table, 18 ), 0, DSIM_INVALID_INDEX ) );

    cr_assert( table->log.commands.count == 2 );
    cr_assert( table->log.commands.data[0].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[0].block == 0 );
    cr_assert( table->log.commands.data[0].start == 20 );
    cr_assert( table->log.commands.data[0].count == 10 );
    cr_assert( table->log.commands.data[1].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[1].block == 0 );
    cr_assert( table->log.commands.data[1].start == 15 );
    cr_assert( table->log.commands.data[1].count == 3 );
}

Test(hash_table_filled, remove_fast)
{
    dsim_table_remove( table, 22, 3 );

    test_hash_table_scheme();
    test_hash_table_count( 7 );

    cr_assert( memcmp( (uint64_t*)dsim_table_data( table, 0, 0 ),     test_data_0,          2*sizeof(test_data_0[0]) ) == 0 );
    cr_assert( memcmp( (uint64_t*)dsim_table_data( table, 0, 0 ) + 2, test_data_0 + 10 - 3, 3*sizeof(test_data_0[0]) ) == 0 );
    cr_assert( memcmp( (uint64_t*)dsim_table_data( table, 0, 0 ) + 5, test_data_0 + 5,      2*sizeof(test_data_0[0]) ) == 0 );

    cr_assert( memcmp( (float*)dsim_table_data( table, 0, 1 ),     test_data_1,          2*sizeof(test_data_1[0]) ) == 0 );
    cr_assert( memcmp( (float*)dsim_table_data( table, 0, 1 ) + 2, test_data_1 + 10 - 3, 3*sizeof(test_data_1[0]) ) == 0 );
    cr_assert( memcmp( (float*)dsim_table_data( table, 0, 1 ) + 5, test_data_1 + 5,      2*sizeof(test_data_1[0]) ) == 0 );

    cr_assert( memcmp( (uint8_t*)dsim_table_data( table, 0, 2 ),         test_data_2,                2*128 ) == 0 );
    cr_assert( memcmp( (uint8_t*)dsim_table_data( table, 0, 2 ) + 2*128, test_data_2 + (10 - 3)*128, 3*128 ) == 0 );
    cr_assert( memcmp( (uint8_t*)dsim_table_data( table, 0, 2 ) + 5*128, test_data_2 + 5*128,        2*128 ) == 0 );

    cr_assert( check_index( dsim_table_find( table, 0 ) , 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 15 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 20 ), 0, 0 ) );
    cr_assert( check_index( dsim_table_find( table, 21 ), 0, 1 ) );
    cr_assert( check_index( dsim_table_find( table, 23 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 26 ), 0, 6 ) );
    cr_assert( check_index( dsim_table_find( table, 28 ), 0, 3 ) );

    cr_assert( table->log.commands.count == 2 );
    cr_assert( table->log.commands.data[0].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[0].block == 0 );
    cr_assert( table->log.commands.data[0].start == 20 );
    cr_assert( table->log.commands.data[0].count == 10 );
    cr_assert( table->log.commands.data[1].type == TCT_REMOVE_FAST );
    cr_assert( table->log.commands.data[1].block == 0 );
    cr_assert( table->log.commands.data[1].start == 2 );
    cr_assert( table->log.commands.data[1].count == 3 );
}

Test(hash_table_filled, remove_ordered)
{
    dsim_table_remove( table, 23, 4 );

    test_hash_table_scheme();
    test_hash_table_count( 6 );

    cr_assert( memcmp( (uint64_t*)dsim_table_data( table, 0, 0 ),     test_data_0,     3*sizeof(test_data_0[0]) ) == 0 );
    cr_assert( memcmp( (uint64_t*)dsim_table_data( table, 0, 0 ) + 3, test_data_0 + 7, 3*sizeof(test_data_0[0]) ) == 0 );

    cr_assert( memcmp( (float*)dsim_table_data( table, 0, 1 ),     test_data_1,     3*sizeof(test_data_1[0]) ) == 0 );
    cr_assert( memcmp( (float*)dsim_table_data( table, 0, 1 ) + 3, test_data_1 + 7, 3*sizeof(test_data_1[0]) ) == 0 );

    cr_assert( memcmp( (uint8_t*)dsim_table_data( table, 0, 2 ),         test_data_2,         3*128 ) == 0 );
    cr_assert( memcmp( (uint8_t*)dsim_table_data( table, 0, 2 ) + 3*128, test_data_2 + 7*128, 3*128 ) == 0 );

    cr_assert( check_index( dsim_table_find( table, 0 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 15 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 20 ), 0, 0 ) );
    cr_assert( check_index( dsim_table_find( table, 22 ), 0, 2 ) );
    cr_assert( check_index( dsim_table_find( table, 24 ), 0, DSIM_INVALID_INDEX ) );
    cr_assert( check_index( dsim_table_find( table, 27 ), 0, 3 ) );
    cr_assert( check_index( dsim_table_find( table, 29 ), 0, 5 ) );

    cr_assert( table->log.commands.count == 2 );
    cr_assert( table->log.commands.data[0].type == TCT_PUSH_BACK );
    cr_assert( table->log.commands.data[0].block == 0 );
    cr_assert( table->log.commands.data[0].start == 20 );
    cr_assert( table->log.commands.data[0].count == 10 );
    cr_assert( table->log.commands.data[1].type == TCT_REMOVE_FAST );
    cr_assert( table->log.commands.data[1].block == 0 );
    cr_assert( table->log.commands.data[1].start == 3 );
    cr_assert( table->log.commands.data[1].count == 4 );
}

Test(hash_table_filled, reset)
{
    dsim_table_reset( table );

    test_hash_table_scheme();
    test_hash_table_empty();
}
*/

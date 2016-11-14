
#include <criterion/criterion.h>

#include "table/hash_table.h"

static struct dsim_table_column table_columns[] = {
    dsim_table_column_static_init( sizeof(uint64_t), &dsim_default_allocator ),
    dsim_table_column_static_init( sizeof(float),    &dsim_default_allocator ),
    dsim_table_column_static_init( 128,              &dsim_default_allocator )
};

static struct dsim_hash_table table = dsim_hash_table_static_init(
        count_of(table_columns), table_columns,
        &dsim_default_allocator );

/*
 * Empty table
 */

void init_hash_table_empty()
{

}

void done_hash_table_empty()
{
    dsim_table_reset( &table.table );
}

TestSuite(hash_table_empty, .init = init_hash_table_empty, .fini = done_hash_table_empty );

Test(hash_table_empty, assert_empty)
{
    cr_assert( dsim_table_column_count( &table.table ) == count_of(table_columns) );
    for( uint32_t i = 0; i < count_of(table.columns); ++i )
        cr_assert( dsim_table_column_size( &table.table, i ) == table_columns[i].width );

    cr_assert( dsim_table_block_count( &table.table) == 1 );
    cr_assert( dsim_table_block_size( &table.table, 0 ) == 0 );

    cr_assert( dsim_table_id_data( &table.table, 0 ) == 0 );
    for( uint32_t i = 0; i < count_of(table.columns); ++i )
        cr_assert( dsim_table_data( &table.table, 0, i ) == 0 );

    cr_assert( dsim_table_find( &table.table, 0 ) == DSIM_INVALID_INDEX );
    cr_assert( dsim_table_find( &table.table, 1 ) == DSIM_INVALID_INDEX );
    cr_assert( dsim_table_find( &table.table, 30 ) == DSIM_INVALID_INDEX );
    cr_assert( dsim_table_find( &table.table, -1 ) == DSIM_INVALID_INDEX );
}

Test(hash_table_empty, insert)
{
    dsim_table_insert( &table.table, 100, 4 );

    cr_assert( dsim_table_block_size( &table.table, 0 ) == 4 );
    cr_assert( dsim_table_id_data( &table.table, 0 ) != 0 );
    for( int i = 0; i < 4; ++i )
        cr_assert( dsim_table_id_data( &table.table, 0 )[i] == 100 + i );

    for( uint32_t i = 0; i < count_of(table.columns); ++i )
        cr_assert( dsim_table_data( &table.table, 0, i ) != 0 );
}

/*
 * Filled table
 */

static const uint32_t test_count = 10;
static const uint64_t test_data_0[] = { 12, 64, 23, 76, 643, 0, 2, 9, 7234, 36 };
static const float    test_data_1[] = { 0, -1, 23.2, 0.64, -0.0001, -0, -2634.6, 54, 0, 6024 };
static uint8_t test_data_2[10*128];

void init_hash_table_filled()
{
    assert( count_of(test_data_0) == test_count );
    assert( count_of(test_data_1) == test_count );
    assert( sizeof(test_data_2) == test_count*128 );
    for( size_t i = 0; i < sizeof(test_data_2); ++i )
        test_data_2[i] = rand() & 0xff;

    dsim_table_insert( &table.table, 20, test_count );
    memcpy( dsim_table_data( &table.table, 0, 0 ), test_data_0, sizeof(test_data_0) );
    memcpy( dsim_table_data( &table.table, 0, 1 ), test_data_0, sizeof(test_data_1) );
    memcpy( dsim_table_data( &table.table, 0, 2 ), test_data_0, sizeof(test_data_2) );
}

void done_hash_table_filled()
{
    dsim_table_reset( &table.table );
}

TestSuite(hash_table_filled, .init = init_hash_table_filled, .fini = done_hash_table_filled );

Test(hash_table_filled, assert_filled)
{
    cr_assert( dsim_table_column_count( &table.table ) == count_of(table_columns) );
    for( uint32_t i = 0; i < count_of(table.columns); ++i )
        cr_assert( dsim_table_column_size( &table.table, i ) == table_columns[i].width );

    cr_assert( dsim_table_block_count( &table.table) == 1 );
    cr_assert( dsim_table_block_size( &table.table, 0 ) == test_count );

    cr_assert( dsim_table_id_data( &table.table, 0 ) != 0 );
    for( uint32_t i = 0; i < count_of(table.columns); ++i )
        cr_assert( dsim_table_data( &table.table, 0, i ) != 0 );

    cr_assert( dsim_table_find( &table.table, 0 ) == DSIM_INVALID_INDEX );
    cr_assert( dsim_table_find( &table.table, 1 ) == DSIM_INVALID_INDEX );
    cr_assert( dsim_table_find( &table.table, 20 ) == 0 );
    cr_assert( dsim_table_find( &table.table, 27 ) == 7 );
}

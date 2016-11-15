
#include <criterion/criterion.h>

#include "table/table_log.h"

#include <stdio.h>

static struct dsim_table_log tlog = dsim_table_log_static_init(&dsim_default_allocator);

/*
 * Empty log
 */

static void init_table_log_empty()
{

}

static void done_table_log_empty()
{
    dsim_table_log_reset( &tlog );
}

TestSuite(table_log_empty, .init = init_table_log_empty, .fini = done_table_log_empty );

Test(table_log_empty, assert_empty)
{
    cr_assert( tlog.version == 0 );
    cr_assert( tlog.commands.data == 0 );
    cr_assert( tlog.commands.capacity == 0 );
    cr_assert( tlog.commands.count == 0 );
}

Test(table_log_empty, add_commands)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );
    dsim_table_log_cmd_push_back( &tlog, 1, 100, 20 );

    cr_assert( tlog.version == 0 );
    cr_assert( tlog.commands.data != 0 );
    cr_assert( tlog.commands.count == 2 );

    cr_assert( tlog.commands.data[0].type == TCT_REMOVE_FAST );
    cr_assert( tlog.commands.data[0].block == 2 );
    cr_assert( tlog.commands.data[0].start == 5 );
    cr_assert( tlog.commands.data[0].count == 10 );

    cr_assert( tlog.commands.data[1].type == TCT_PUSH_BACK );
    cr_assert( tlog.commands.data[1].block == 1 );
    cr_assert( tlog.commands.data[1].start == 100 );
    cr_assert( tlog.commands.data[1].count == 20 );
}

Test(table_log_empty, shrink_empty)
{
    dsim_table_log_shrink( &tlog );
    cr_assert( tlog.version == 0 );
    cr_assert( tlog.commands.data == 0 );
    cr_assert( tlog.commands.capacity == 0 );
    cr_assert( tlog.commands.count == 0 );
}

Test(table_log_empty, shrink_some)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );
    dsim_table_log_cmd_push_back( &tlog, 1, 100, 20 );
    dsim_table_log_shrink( &tlog );

    cr_assert( tlog.version == 2 );
    cr_assert( tlog.commands.count == 0 );
}

Test(table_log_empty, reset)
{
    dsim_table_log_reset( &tlog );
    cr_assert( tlog.version == 0 );
    cr_assert( tlog.commands.data == 0 );
    cr_assert( tlog.commands.capacity == 0 );
    cr_assert( tlog.commands.count == 0 );
}

/*
 * Filled log
 */

static void init_table_log_filled()
{
    dsim_table_log_cmd_push_back( &tlog, 0, 1000, 250 );
    dsim_table_log_cmd_push_back( &tlog, 0, 2000, 100 );
    dsim_table_log_cmd_push_back( &tlog, 0, 1, 5 );
    dsim_table_log_cmd_push_back( &tlog, 0, 6, 15 );
    dsim_table_log_shrink( &tlog );
    dsim_table_log_cmd_remove_fast( &tlog, 0, 250, 100 );
    dsim_table_log_cmd_push_back( &tlog, 0, 2000, 50 );
}

static void done_table_log_filled()
{
    dsim_table_log_reset( &tlog );
}

TestSuite(table_log_filled, .init = init_table_log_filled, .fini = done_table_log_filled );

Test(table_log_filled, assert_filled)
{
    cr_assert( tlog.version == 4 );
    cr_assert( tlog.commands.data != 0 );
    cr_assert( tlog.commands.count == 2 );

    cr_assert( tlog.commands.data[0].type == TCT_REMOVE_FAST );
    cr_assert( tlog.commands.data[0].block == 0 );
    cr_assert( tlog.commands.data[0].start == 250 );
    cr_assert( tlog.commands.data[0].count == 100 );

    cr_assert( tlog.commands.data[1].type == TCT_PUSH_BACK );
    cr_assert( tlog.commands.data[1].block == 0 );
    cr_assert( tlog.commands.data[1].start == 2000 );
    cr_assert( tlog.commands.data[1].count == 50 );
}

Test(table_log_filled, add_commands)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );

    cr_assert( tlog.version == 4 );
    cr_assert( tlog.commands.count == 3 );

    cr_assert( tlog.commands.data[0].type == TCT_REMOVE_FAST );
    cr_assert( tlog.commands.data[0].block == 0 );
    cr_assert( tlog.commands.data[0].start == 250 );
    cr_assert( tlog.commands.data[0].count == 100 );

    cr_assert( tlog.commands.data[1].type == TCT_PUSH_BACK );
    cr_assert( tlog.commands.data[1].block == 0 );
    cr_assert( tlog.commands.data[1].start == 2000 );
    cr_assert( tlog.commands.data[1].count == 50 );

    cr_assert( tlog.commands.data[2].type == TCT_REMOVE_FAST );
    cr_assert( tlog.commands.data[2].block == 2 );
    cr_assert( tlog.commands.data[2].start == 5 );
    cr_assert( tlog.commands.data[2].count == 10 );
}

Test(table_log_filled, shrink)
{
    dsim_table_log_shrink( &tlog );

    cr_assert( tlog.version == 6 );
    cr_assert( tlog.commands.count == 0 );
}

Test(table_log_filled, reset)
{
    dsim_table_log_reset( &tlog );
    cr_assert( tlog.version == 0 );
    cr_assert( tlog.commands.data == 0 );
    cr_assert( tlog.commands.capacity == 0 );
    cr_assert( tlog.commands.count == 0 );
}

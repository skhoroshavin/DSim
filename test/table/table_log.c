
#include "unity_fixture.h"
#include "test_allocator.h"

#include "table/table_log.h"

static struct dsim_table_log tlog = dsim_table_log_static_init(&dsim_test_allocator);

/*
 * Empty log
 */

TEST_GROUP(table_log_empty);

TEST_SETUP(table_log_empty)
{

}

TEST_TEAR_DOWN(table_log_empty)
{
    dsim_table_log_reset( &tlog );
}

TEST(table_log_empty, assert_empty)
{
    TEST_ASSERT_EQUAL( tlog.version, 0 );
    TEST_ASSERT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST(table_log_empty, add_commands)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );
    dsim_table_log_cmd_push_back( &tlog, 1, 100, 20 );

    TEST_ASSERT_EQUAL( tlog.version, 0 );
    TEST_ASSERT_NOT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.count, 2 );

    TEST_ASSERT_EQUAL( tlog.commands.at[0].type, TCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].block, 2 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].start, 5 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].count, 10 );

    TEST_ASSERT_EQUAL( tlog.commands.at[1].type, TCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].block, 1 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].start, 100 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].count, 20 );
}

TEST(table_log_empty, shrink_empty)
{
    dsim_table_log_shrink( &tlog );
    TEST_ASSERT_EQUAL( tlog.version, 0 );
    TEST_ASSERT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST(table_log_empty, shrink_some)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );
    dsim_table_log_cmd_push_back( &tlog, 1, 100, 20 );
    dsim_table_log_shrink( &tlog );

    TEST_ASSERT_EQUAL( tlog.version, 2 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST(table_log_empty, reset)
{
    dsim_table_log_reset( &tlog );
    TEST_ASSERT_EQUAL( tlog.version, 0 );
    TEST_ASSERT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST_GROUP_RUNNER(table_log_empty)
{
    RUN_TEST_CASE(table_log_empty, assert_empty);
    RUN_TEST_CASE(table_log_empty, add_commands);
    RUN_TEST_CASE(table_log_empty, shrink_empty);
    RUN_TEST_CASE(table_log_empty, shrink_some);
    RUN_TEST_CASE(table_log_empty, reset);
}

/*
 * Non empty log
 */

TEST_GROUP(table_log_non_empty);

TEST_SETUP(table_log_non_empty)
{
    dsim_table_log_cmd_push_back( &tlog, 0, 1000, 250 );
    dsim_table_log_cmd_push_back( &tlog, 0, 2000, 100 );
    dsim_table_log_cmd_push_back( &tlog, 0, 1, 5 );
    dsim_table_log_cmd_push_back( &tlog, 0, 6, 15 );
    dsim_table_log_shrink( &tlog );
    dsim_table_log_cmd_remove_fast( &tlog, 0, 250, 100 );
    dsim_table_log_cmd_push_back( &tlog, 0, 2000, 50 );
}

TEST_TEAR_DOWN(table_log_non_empty)
{
    dsim_table_log_reset( &tlog );
}

TEST(table_log_non_empty, assert_non_empty)
{
    TEST_ASSERT_EQUAL( tlog.version, 4 );
    TEST_ASSERT_NOT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.count, 2 );

    TEST_ASSERT_EQUAL( tlog.commands.at[0].type, TCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].start, 250 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].count, 100 );

    TEST_ASSERT_EQUAL( tlog.commands.at[1].type, TCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].start, 2000 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].count, 50 );
}

TEST(table_log_non_empty, add_commands)
{
    dsim_table_log_cmd_remove_fast( &tlog, 2, 5, 10 );

    TEST_ASSERT_EQUAL( tlog.version, 4 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 3 );

    TEST_ASSERT_EQUAL( tlog.commands.at[0].type, TCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].start, 250 );
    TEST_ASSERT_EQUAL( tlog.commands.at[0].count, 100 );

    TEST_ASSERT_EQUAL( tlog.commands.at[1].type, TCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].start, 2000 );
    TEST_ASSERT_EQUAL( tlog.commands.at[1].count, 50 );

    TEST_ASSERT_EQUAL( tlog.commands.at[2].type, TCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( tlog.commands.at[2].block, 2 );
    TEST_ASSERT_EQUAL( tlog.commands.at[2].start, 5 );
    TEST_ASSERT_EQUAL( tlog.commands.at[2].count, 10 );
}

TEST(table_log_non_empty, shrink)
{
    dsim_table_log_shrink( &tlog );

    TEST_ASSERT_EQUAL( tlog.version, 6 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST(table_log_non_empty, reset)
{
    dsim_table_log_reset( &tlog );
    TEST_ASSERT_EQUAL( tlog.version, 0 );
    TEST_ASSERT_NULL( tlog.commands.data );
    TEST_ASSERT_EQUAL( tlog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( tlog.commands.count, 0 );
}

TEST_GROUP_RUNNER(table_log_non_empty)
{
    RUN_TEST_CASE(table_log_non_empty, assert_non_empty);
    RUN_TEST_CASE(table_log_non_empty, add_commands);
    RUN_TEST_CASE(table_log_non_empty, shrink);
    RUN_TEST_CASE(table_log_non_empty, reset);
}

void run_test_table_log()
{
    RUN_TEST_GROUP(table_log_empty);
    RUN_TEST_GROUP(table_log_non_empty);
}

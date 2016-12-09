
#include "unity_fixture.h"
#include "test_allocator.h"

#include "storage/storage_log.h"

static struct dsim_storage_log slog = dsim_storage_log_static_init(&dsim_test_allocator);

/*
 * Empty log
 */

TEST_GROUP(storage_log_empty);

TEST_SETUP(storage_log_empty)
{

}

TEST_TEAR_DOWN(storage_log_empty)
{
    dsim_storage_log_reset( &slog );
}

TEST(storage_log_empty, assert_empty)
{
    TEST_ASSERT_EQUAL( slog.version, 0 );
    TEST_ASSERT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST(storage_log_empty, add_commands)
{
    dsim_storage_log_cmd_remove_fast( &slog, 2, 5, 10 );
    dsim_storage_log_cmd_push_back( &slog, 1, 100, 20 );

    TEST_ASSERT_EQUAL( slog.version, 0 );
    TEST_ASSERT_NOT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.count, 2 );

    TEST_ASSERT_EQUAL( slog.commands.at[0].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( slog.commands.at[0].block, 2 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].start, 5 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].count, 10 );

    TEST_ASSERT_EQUAL( slog.commands.at[1].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( slog.commands.at[1].block, 1 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].start, 100 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].count, 20 );
}

TEST(storage_log_empty, shrink_empty)
{
    dsim_storage_log_shrink( &slog );
    TEST_ASSERT_EQUAL( slog.version, 0 );
    TEST_ASSERT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST(storage_log_empty, shrink_some)
{
    dsim_storage_log_cmd_remove_fast( &slog, 2, 5, 10 );
    dsim_storage_log_cmd_push_back( &slog, 1, 100, 20 );
    dsim_storage_log_shrink( &slog );

    TEST_ASSERT_EQUAL( slog.version, 2 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST(storage_log_empty, reset)
{
    dsim_storage_log_reset( &slog );
    TEST_ASSERT_EQUAL( slog.version, 0 );
    TEST_ASSERT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST_GROUP_RUNNER(storage_log_empty)
{
    RUN_TEST_CASE(storage_log_empty, assert_empty);
    RUN_TEST_CASE(storage_log_empty, add_commands);
    RUN_TEST_CASE(storage_log_empty, shrink_empty);
    RUN_TEST_CASE(storage_log_empty, shrink_some);
    RUN_TEST_CASE(storage_log_empty, reset);
}

/*
 * Non empty log
 */

TEST_GROUP(storage_log_non_empty);

TEST_SETUP(storage_log_non_empty)
{
    dsim_storage_log_cmd_push_back( &slog, 0, 1000, 250 );
    dsim_storage_log_cmd_push_back( &slog, 0, 2000, 100 );
    dsim_storage_log_cmd_push_back( &slog, 0, 1, 5 );
    dsim_storage_log_cmd_push_back( &slog, 0, 6, 15 );
    dsim_storage_log_shrink( &slog );
    dsim_storage_log_cmd_remove_fast( &slog, 0, 250, 100 );
    dsim_storage_log_cmd_push_back( &slog, 0, 2000, 50 );
}

TEST_TEAR_DOWN(storage_log_non_empty)
{
    dsim_storage_log_reset( &slog );
}

TEST(storage_log_non_empty, assert_non_empty)
{
    TEST_ASSERT_EQUAL( slog.version, 4 );
    TEST_ASSERT_NOT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.count, 2 );

    TEST_ASSERT_EQUAL( slog.commands.at[0].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( slog.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].start, 250 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].count, 100 );

    TEST_ASSERT_EQUAL( slog.commands.at[1].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( slog.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].start, 2000 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].count, 50 );
}

TEST(storage_log_non_empty, add_commands)
{
    dsim_storage_log_cmd_remove_fast( &slog, 2, 5, 10 );

    TEST_ASSERT_EQUAL( slog.version, 4 );
    TEST_ASSERT_EQUAL( slog.commands.count, 3 );

    TEST_ASSERT_EQUAL( slog.commands.at[0].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( slog.commands.at[0].block, 0 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].start, 250 );
    TEST_ASSERT_EQUAL( slog.commands.at[0].count, 100 );

    TEST_ASSERT_EQUAL( slog.commands.at[1].type, SCT_PUSH_BACK );
    TEST_ASSERT_EQUAL( slog.commands.at[1].block, 0 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].start, 2000 );
    TEST_ASSERT_EQUAL( slog.commands.at[1].count, 50 );

    TEST_ASSERT_EQUAL( slog.commands.at[2].type, SCT_REMOVE_FAST );
    TEST_ASSERT_EQUAL( slog.commands.at[2].block, 2 );
    TEST_ASSERT_EQUAL( slog.commands.at[2].start, 5 );
    TEST_ASSERT_EQUAL( slog.commands.at[2].count, 10 );
}

TEST(storage_log_non_empty, shrink)
{
    dsim_storage_log_shrink( &slog );

    TEST_ASSERT_EQUAL( slog.version, 6 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST(storage_log_non_empty, reset)
{
    dsim_storage_log_reset( &slog );
    TEST_ASSERT_EQUAL( slog.version, 0 );
    TEST_ASSERT_NULL( slog.commands.data );
    TEST_ASSERT_EQUAL( slog.commands.capacity, 0 );
    TEST_ASSERT_EQUAL( slog.commands.count, 0 );
}

TEST_GROUP_RUNNER(storage_log_non_empty)
{
    RUN_TEST_CASE(storage_log_non_empty, assert_non_empty);
    RUN_TEST_CASE(storage_log_non_empty, add_commands);
    RUN_TEST_CASE(storage_log_non_empty, shrink);
    RUN_TEST_CASE(storage_log_non_empty, reset);
}

void run_test_storage_log()
{
    RUN_TEST_GROUP(storage_log_empty);
    RUN_TEST_GROUP(storage_log_non_empty);
}


#include "unity_fixture.h"

extern void run_test_common();

extern void run_test_allocator();

extern void run_test_array();
extern void run_test_hash();

extern void run_test_table_log();
extern void run_test_hash_table();

static void run_all_tests()
{
    run_test_common();

    run_test_allocator();

    run_test_array();
    run_test_hash();

    run_test_table_log();
    run_test_hash_table();
}

int main( int argc, const char * argv[] )
{
    return UnityMain( argc, argv, run_all_tests );
}

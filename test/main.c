
#include "unity_fixture.h"

#include "containers/test_array.h"
#include "containers/test_hash.h"

void run_test_common();

void run_test_allocator();

void run_test_table_log();
void run_test_hash_table();

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

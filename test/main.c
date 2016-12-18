
#include <time.h>

#include "containers/test_array.h"
#include "containers/test_hash.h"

#include "storage/test_storage_array.h"

void run_test_common();

void run_test_allocator();

void run_test_storage_log();
void run_test_hash_storage();

static void run_all_tests()
{
    run_test_common();

    run_test_allocator();

    run_test_array();
    run_test_hash();

    run_test_storage_array();

    run_test_storage_log();
    run_test_hash_storage();
}

int main( int argc, const char *argv[] )
{
    srand( time(0) );
    return UnityMain( argc, argv, run_all_tests );
}

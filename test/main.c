
#include <time.h>

#include "core/test_array.h"
#include "core/test_hash.h"

#include "storage/test_storage_array.h"

SUITE_EXTERN(test_common);

SUITE_EXTERN(test_allocator);

SUITE_EXTERN(test_hash_storage);

GREATEST_MAIN_DEFS();

int main( int argc, char **argv )
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(test_common);
    RUN_SUITE(test_allocator);

    RUN_SUITE(test_array);
    RUN_SUITE(test_hash);

    RUN_SUITE(test_storage_array);

    RUN_SUITE(test_hash_storage);

    GREATEST_MAIN_END();
}

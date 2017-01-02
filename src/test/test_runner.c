
#include "test_runner.h"
#include "core/stack_allocator.h"

GREATEST_MAIN_DEFS();

void dsim_run_test( const char *name, dsim_rand_test test )
{
    if( greatest_pre_test(name) == 1 )
    {
        enum greatest_test_res res = GREATEST_TEST_RES_PASS;

        for( size_t i = 0; i < 100; ++i )
        {
            struct dsim_random_test_data data;

            dsim_random_test_data_init( &data, &dsim_default_allocator );
            res = test( &data.base );
            dsim_random_test_data_done( &data );

            if( res == GREATEST_TEST_RES_FAIL )
                break;
        }
        greatest_post_test( name, res );
    }
    else if(GREATEST_LIST_ONLY())
        fprintf( GREATEST_STDOUT, "  %s\n", name );
}

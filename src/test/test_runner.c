
#include "test_runner.h"
#include "test_context.h"
#include "core/stack_allocator.h"

GREATEST_MAIN_DEFS();

void dsim_run_test( const char *name, dsim_rand_test test )
{
    if( greatest_pre_test(name) == 1 )
    {
        enum greatest_test_res res = GREATEST_TEST_RES_PASS;

        for( size_t i = 0; i < 100; ++i )
        {
            struct dsim_random_test_context data;

            dsim_random_test_context_init( &data, &dsim_default_allocator );
            res = test( &data.base );

            if( res == GREATEST_TEST_RES_FAIL )
            {
                dsim_test_context_dump( &data.base );
                dsim_random_test_context_done( &data );
                break;
            }

            dsim_random_test_context_done( &data );
        }
        greatest_post_test( name, res );
    }
    else if(GREATEST_LIST_ONLY())
        fprintf( GREATEST_STDOUT, "  %s\n", name );
}

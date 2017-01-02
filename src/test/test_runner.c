
#include "test_runner.h"

GREATEST_MAIN_DEFS();

void dsim_run_test( const char *name, dsim_rand_test test )
{
    for( size_t i = 0; i < 100; ++i )
    {
        enum greatest_test_res res = GREATEST_TEST_RES_PASS;
        struct dsim_random_test_data data;
        dsim_random_test_data_init( &data );

        if( greatest_pre_test(name) == 1 )
        {
            res = test( &data.base );
            greatest_post_test( name, res );
        }

        dsim_random_test_data_done( &data );

        if( res == GREATEST_TEST_RES_FAIL )
            return;
    }

    if(GREATEST_LIST_ONLY())
        fprintf( GREATEST_STDOUT, "  %s\n", name );
}

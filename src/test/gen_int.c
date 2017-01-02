
#include "gen_int.h"

unsigned test_gen_uint( struct dsim_test_data *data, unsigned min, unsigned max )
{
    assert( min <= max ); //  LCOV_EXCL_BR_LINE

    unsigned result;
    dsim_test_data_gen( data, &result, sizeof(result) );
    if( (unsigned)(max - min) < UINT32_MAX )
        result %= max - min + 1;
    result += min;
    return result;
}

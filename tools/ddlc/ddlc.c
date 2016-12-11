
#include "reflection/ddl_reader.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_json_parser.h"
#include "reflection/ddl_utils.h"

#include "containers/array.h"
#include "containers/string.h"
#include "reflection/ddl_registry.h"
#include "utils/file.h"
#include "utils/log.h"

#include "ddl_json.h"
#include "ddl_c.h"

#include <stdio.h>

static void * load_json_ddl( const char * filename, size_t * size_out )
{
    struct dsim_array_char json = dsim_array_static_init( &dsim_default_allocator );
    dsim_read_file( filename, &json );

    void *buf = dsim_ddl_compile_json( json.data, json.count, size_out );
    dsim_array_char_reset( &json );
    return buf;
}

int main( int argc, char *argv[] )
{
    size_t size;
    void *data = load_json_ddl( argv[1], &size );
    if( !data )
        dsim_fatal( "Failed to load %s", argv[1] );

    struct dsim_string bin_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &bin_name, argv[1] );
    dsim_string_append( &bin_name, ".bin" );
    FILE * f = fopen( bin_name.data, "w" );
    dsim_string_reset( &bin_name );
    fwrite( data, 1, size, f );
    fclose( f );

    dsim_ddl_generate_c( argv[1], data, size );

    free( data );

    dsim_ddl_registry_reset();

    return 0;
}

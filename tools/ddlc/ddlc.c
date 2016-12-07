
#include "reflection/ddl_reader.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_json_parser.h"

#include "containers/array.h"
#include "containers/string.h"
#include "reflection/ddl_registry.h"
#include "utils/file.h"
#include "ddl_types.h"

#include <stdio.h>

static struct dsim_array_ptr buffers = dsim_array_static_init(&dsim_default_allocator);

static void * load_json_ddl( const char * filename )
{
    struct dsim_array_uint8 json = dsim_array_static_init( &dsim_default_allocator );
    dsim_read_file( filename, &json );

    struct flatcc_builder b;
    flatcc_builder_init( &b );

    struct flatcc_json_parser_ctx parser;
    void *buf = 0;

    int err = ddl_parse_json( &b, &parser, (const char*)json.data, json.count, 0 );
    if( err )
    {
        printf( "Line %d: %s\n", parser.line, flatcc_json_parser_error_string(err) );
        goto finally;
    }

    buf = flatcc_builder_finalize_buffer( &b, 0 );

finally:
    flatcc_builder_clear( &b );
    dsim_array_uint8_reset( &json );
    return buf;
}

static void rebuild_ddl( dsim_ddl_table_t ddl )
{
    dsim_type_vec_t types = dsim_ddl_types( ddl );
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
    {
        struct flatcc_builder b;
        flatcc_builder_init( &b );

        dsim_type_start_as_root( &b );
        dsim_rebuild_type( &b, dsim_type_vec_at(types, i) );
        dsim_type_end_as_root( &b );

        void *buf = flatcc_builder_finalize_buffer( &b, 0 );
        flatcc_builder_clear( &b );
        dsim_array_ptr_push_back( &buffers, buf );

        dsim_type_table_t new_type = dsim_type_as_root(buf);
        dsim_register_type( new_type );
    }
}

static void * clone_ddl( dsim_ddl_table_t ddl, size_t * size_out )
{
    struct flatcc_builder b;
    flatcc_builder_init( &b );
    void *buf = 0;

    dsim_ddl_start_as_root( &b );

    dsim_ddl_name_clone( &b, dsim_ddl_name(ddl) );

    dsim_ddl_types_start( &b );
    dsim_type_vec_t types = dsim_ddl_types( ddl );
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
    {
        dsim_type_table_t type = dsim_type_vec_at(types, i);

        dsim_ddl_types_push_start( &b );
        dsim_rebuild_type( &b, type );
        dsim_ddl_types_push_end( &b );
    }
    dsim_ddl_types_end( &b );

    dsim_ddl_end_as_root( &b );

    buf = flatcc_builder_finalize_buffer( &b, size_out );
    flatcc_builder_clear( &b );
    return buf;
}

static void dump_c_array( FILE * f, const char * data, size_t size )
{
    for( size_t i = 0; i < size; ++i )
    {
        if( i % 16 == 0 ) fprintf( f, "    " );
        fprintf( f, "%d", data[i] );
        if( i+1 == size )
        {
            fprintf( f, "\n" );
            break;
        }
        fprintf( f, "," );
        if( i % 16 == 15 ) fprintf( f, "\n" );
        else fprintf( f, " " );
    }
}

int main( int argc, char * argv[] )
{
    void * data = load_json_ddl( argv[1] );
    dsim_ddl_table_t ddl = dsim_ddl_as_root( data );

    rebuild_ddl( ddl );

    size_t new_size;
    char *new_data = (char*)clone_ddl( ddl, &new_size );
    ddl = dsim_ddl_as_root( new_data );

    struct dsim_string bin_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &bin_name, argv[1] );
    dsim_string_append( &bin_name, ".bin" );

    struct dsim_string h_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &h_name, argv[1] );
    dsim_string_append( &h_name, ".h" );

    struct dsim_string c_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &c_name, argv[1] );
    dsim_string_append( &c_name, ".c" );

    FILE * f = fopen( bin_name.data, "w" );
    fwrite( new_data, 1, new_size, f );
    fclose( f );

    f = fopen( h_name.data, "w" );
    fprintf( f, "\n" );
    fprintf( f, "#pragma once\n" );
    fprintf( f, "\n" );
    fprintf( f, "extern const char dsim_ddl_%s_data[];\n", dsim_ddl_name(ddl) );
    fprintf( f, "\n" );
    fprintf( f, "void dsim_ddl_register_%s();\n", dsim_ddl_name(ddl) );
    fprintf( f, "\n" );
    fclose( f );

    f = fopen( c_name.data, "w" );
    fprintf( f, "\n" );
    fprintf( f, "#include \"reflection/ddl_registry.h\"\n" );
    fprintf( f, "#include \"%s\"\n", h_name.data );
    fprintf( f, "\n" );
    fprintf( f, "const char dsim_ddl_%s_data[] = {\n", dsim_ddl_name(ddl) );
    dump_c_array( f, new_data, new_size );
    fprintf( f, "};\n" );
    fprintf( f, "\n" );
    fprintf( f, "void dsim_ddl_register_%s()\n", dsim_ddl_name(ddl) );
    fprintf( f, "{\n" );
    fprintf( f, "    dsim_ddl_register( dsim_ddl_%s_data );\n", dsim_ddl_name(ddl) );
    fprintf( f, "}\n" );
    fclose( f );

    dsim_string_reset( &bin_name );
    dsim_string_reset( &h_name );
    dsim_string_reset( &c_name );

    free( new_data );
    free( data );

    dsim_ddl_registry_reset();
    for( uint32_t i = 0; i < buffers.count; ++i )
        free( buffers.at[i] );
    dsim_array_ptr_reset( &buffers );

    return 0;
}

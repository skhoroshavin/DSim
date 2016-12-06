
#include "reflection/ddl_reader.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_json_parser.h"

#include "containers/array.h"

#include <stdio.h>

static void read_file( const char * filename, struct dsim_array_uint8 * data )
{
    const int buf_size = 4096;
    uint8_t buf[buf_size];

    FILE * f = fopen( filename, "rb" );
    int read_size;
    do
    {
        read_size = fread( buf, 1, buf_size, f );
        dsim_array_uint8_push_back_n( data, buf, read_size );
    } while( read_size == buf_size );
    fclose( f );
}

static void * load_json_ddl( const char * filename )
{
    struct dsim_array_uint8 json = dsim_array_static_init( &dsim_default_allocator );
    read_file( filename, &json );

    struct flatcc_builder builder;
    flatcc_builder_init( &builder );

    struct flatcc_json_parser_ctx ctx;
    void *buf = 0;

    int err = ddl_parse_json( &builder, &ctx, (const char*)json.data, json.count, 0 );
    if( err )
    {
        printf( "Line %d: %s\n", ctx.line, flatcc_json_parser_error_string(err) );
        goto finally;
    }

    buf = flatcc_builder_finalize_buffer( &builder, 0 );

finally:
    flatcc_builder_clear( &builder );
    dsim_array_uint8_reset( &json );
    return buf;
}

static void dump_ddl( dsim_ddl_table_t ddl )
{
    printf( "Types:\n" );
    dsim_type_vec_t types = dsim_ddl_types( ddl );
    for( size_t i = 0; i != dsim_type_vec_len(types); ++i )
    {
        dsim_type_table_t type = dsim_type_vec_at(types, i);
        printf( "  %s, %s, size %d, align %d\n",
                dsim_type_name(type),
                dsim_any_type_type_name( dsim_type_data_type(type) ),
                dsim_type_size(type),
                dsim_type_align(type) );
        switch( dsim_type_data_type(type) )
        {
        case dsim_any_type_numeric_type:
        {
            dsim_numeric_type_table_t ntype = (dsim_numeric_type_table_t)dsim_type_data( type );
            if( dsim_numeric_type_is_float(ntype) )
                printf( "    Floating point\n" );
            else
                printf( "    Integer\n" );
            break;
        }
        case dsim_any_type_struct_type:
        {
            dsim_struct_type_table_t stype = (dsim_struct_type_table_t)dsim_type_data( type );
            dsim_struct_field_vec_t fields = dsim_struct_type_fields( stype );
            for( size_t j = 0; j != dsim_struct_field_vec_len(fields); ++j )
            {
                dsim_struct_field_table_t field = dsim_struct_field_vec_at(fields,j);
                printf( "    %s: %s, offset %d\n",
                        dsim_struct_field_name(field),
                        dsim_struct_field_type(field),
                        dsim_struct_field_offset(field) );
            }
            break;
        }
        case dsim_any_type_enum_type:
        {
            dsim_enum_type_table_t stype = (dsim_enum_type_table_t)dsim_type_data( type );
            flatbuffers_string_vec_t values = dsim_enum_type_values( stype );
            for( size_t j = 0; j != flatbuffers_string_vec_len(values); ++j )
                printf( "    %s\n", flatbuffers_string_vec_at(values, j) );
            break;
        }
        case dsim_any_type_reference_type:
        {
            dsim_reference_type_table_t rtype = (dsim_reference_type_table_t)dsim_type_data( type );
            printf( "    Target: %s\n", dsim_reference_type_target(rtype) );
            break;
        }
        }
    }

    printf( "Layouts:\n" );
    dsim_layout_vec_t layouts = dsim_ddl_layouts( ddl );
    for( size_t i = 0; i != dsim_layout_vec_len(layouts); ++i )
    {
        dsim_layout_table_t layout = dsim_layout_vec_at(layouts, i);
        printf( "  %s\n", dsim_layout_name(layout) );
        dsim_column_vec_t columns = dsim_layout_columns(layout);
        for( size_t j = 0; j != dsim_column_vec_len(columns); ++j )
        {
            dsim_column_table_t column = dsim_column_vec_at(columns, j);
            printf( "    %s: %s\n",
                    dsim_column_name(column),
                    dsim_column_type(column) );
        }

    }
}

int main( int argc, char * argv[] )
{
    void * data = load_json_ddl( argv[1] );

    dsim_ddl_table_t ddl = dsim_ddl_as_root( data );
    dump_ddl( ddl );

    free( data );

    return 0;
}

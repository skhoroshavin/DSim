
#include "ddl_json.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_json_parser.h"
#include "reflection/ddl_registry.h"
#include "utils/log.h"

static void *dsim_ddl_load_json( const char *json_data, size_t json_size, size_t *size_out )
{
    struct flatcc_builder b;
    flatcc_builder_init( &b );

    struct flatcc_json_parser_ctx parser;
    void *buf = 0;

    int err = ddl_parse_json( &b, &parser, json_data, json_size, 0 );
    if( err )
    {
        dsim_error( "Line %d: %s\n", parser.line, flatcc_json_parser_error_string(err) );
        goto finally;
    }

    buf = flatcc_builder_finalize_buffer( &b, size_out );

finally:
    flatcc_builder_clear( &b );
    return buf;
}

static void dsim_process_number( struct flatcc_builder * b, dsim_ddl_type_table_t type )
{
    dsim_ddl_numeric_type_table_t ntype = (dsim_ddl_numeric_type_table_t)dsim_ddl_type_data( type );

    // LCOV_EXCL_START
    if( !dsim_ddl_type_size_is_present(type) )
        dsim_fatal( "Numeric type %s must have size!", dsim_ddl_type_name(type) );
    // LCOV_EXCL_END

    int size = dsim_ddl_type_size(type);
    dsim_ddl_type_size_add( b, size );

    int align = dsim_ddl_type_align(type);
    if( !dsim_ddl_type_align_is_present(type) )
        align = min( 16, dsim_next_pow_2(size) );
    dsim_ddl_type_align_add( b, align );

    dsim_ddl_type_data_numeric_type_start( b );
    dsim_ddl_numeric_type_is_float_add( b, dsim_ddl_numeric_type_is_float(ntype) );

    dsim_ddl_type_data_numeric_type_end( b );
}

static void dsim_process_struct( struct flatcc_builder *b, dsim_ddl_type_table_t type, dsim_ddl_type_vec_t prev_types )
{
    dsim_ddl_struct_type_table_t stype = (dsim_ddl_struct_type_table_t)dsim_ddl_type_data( type );

    dsim_ddl_type_data_struct_type_start( b );

    int offset = 0;
    int align = 0;
    dsim_ddl_struct_field_vec_t fields = dsim_ddl_struct_type_fields( stype );
    dsim_ddl_struct_type_fields_start( b );
    for( size_t i = 0; i != dsim_ddl_struct_field_vec_len(fields); ++i )
    {
        dsim_ddl_struct_type_fields_push_start( b );

        dsim_ddl_struct_field_table_t field = dsim_ddl_struct_field_vec_at(fields,i);
        dsim_ddl_struct_field_name_clone( b, dsim_ddl_struct_field_name(field) );
        dsim_ddl_struct_field_type_clone( b, dsim_ddl_struct_field_type(field) );

        if( dsim_ddl_struct_field_offset_is_present(field) )
            offset = dsim_ddl_struct_field_offset(field);
        dsim_ddl_struct_field_offset_add( b, offset );

        if( prev_types )
        {
            dsim_ddl_type_table_t type = dsim_ddl_types_scan_by_name( prev_types, dsim_ddl_struct_field_type(field) );
            offset += dsim_ddl_type_size(type);
            align = max( align, dsim_ddl_type_align(type) );
        }

        dsim_ddl_struct_type_fields_push_end( b );
    }
    dsim_ddl_struct_type_fields_end( b );

    int size = offset;
    while( size & align )
        ++size;

    dsim_ddl_type_data_struct_type_end( b );

    if( dsim_ddl_type_size_is_present(type) )
        size = dsim_ddl_type_size(type);

    if( dsim_ddl_type_align_is_present(type) )
        align = dsim_ddl_type_align(type);

    dsim_ddl_type_size_add( b, size );
    dsim_ddl_type_align_add( b, align );
}

static void dsim_process_enum( struct flatcc_builder *b, dsim_ddl_type_table_t type )
{
    dsim_ddl_enum_type_table_t etype = (dsim_ddl_enum_type_table_t)dsim_ddl_type_data( type );

    dsim_ddl_type_size_add( b, 4 );
    dsim_ddl_type_align_add( b, 4 );

    dsim_ddl_type_data_enum_type_start( b );

    flatbuffers_string_vec_t values = dsim_ddl_enum_type_values( etype );
    dsim_ddl_enum_type_values_start( b );
    for( size_t i = 0; i != flatbuffers_string_vec_len(values); ++i )
        dsim_ddl_enum_type_values_push_clone( b, flatbuffers_string_vec_at(values, i) );
    dsim_ddl_enum_type_values_end( b );

    dsim_ddl_type_data_enum_type_end( b );
}

static void dsim_process_reference( struct flatcc_builder *b, dsim_ddl_type_table_t type )
{
    dsim_ddl_reference_type_table_t rtype = (dsim_ddl_reference_type_table_t)dsim_ddl_type_data( type );

    dsim_ddl_type_size_add( b, 8 );
    dsim_ddl_type_align_add( b, 8 );

    dsim_ddl_type_data_reference_type_start( b );
    dsim_ddl_reference_type_target_clone( b, dsim_ddl_reference_type_target(rtype) );
    dsim_ddl_type_data_reference_type_end( b );
}

static void dsim_process_type( struct flatcc_builder *b, dsim_ddl_type_table_t type, dsim_ddl_type_vec_t prev_types )
{
    dsim_ddl_type_name_clone( b, dsim_ddl_type_name(type) );
    if( dsim_ddl_type_ctype_is_present(type) )
        dsim_ddl_type_ctype_clone( b, dsim_ddl_type_ctype(type) );

    switch( dsim_ddl_type_data_type(type) )
    {
    case dsim_ddl_any_type_numeric_type:
        dsim_process_number( b, type );
        break;
    case dsim_ddl_any_type_struct_type:
        dsim_process_struct( b, type, prev_types );
        break;
    case dsim_ddl_any_type_enum_type:
        dsim_process_enum( b, type );
        break;
    case dsim_ddl_any_type_reference_type:
        dsim_process_reference( b, type );
        break;
    }
}

static void dsim_process_types( struct flatcc_builder *b, dsim_ddl_type_vec_t types, dsim_ddl_type_vec_t prev_types )
{
    dsim_ddl_root_types_start( b );
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);

        dsim_ddl_root_types_push_start( b );
        dsim_process_type( b, type, prev_types );
        dsim_ddl_root_types_push_end( b );
    }
    dsim_ddl_root_types_end( b );
}

static void dsim_process_layouts( struct flatcc_builder *b, dsim_ddl_layout_vec_t layouts )
{
    dsim_ddl_root_layouts_start( b );
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);

        dsim_ddl_root_layouts_push_start( b );
        dsim_ddl_layout_name_clone( b, dsim_ddl_layout_name(layout) );

        dsim_ddl_layout_arrays_start( b );
        dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(layout);
        for( size_t j = 0; j != dsim_ddl_array_vec_len(arrays); ++j )
        {
            dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays, j);

            dsim_ddl_layout_arrays_push_start( b );
            dsim_ddl_array_name_clone( b, dsim_ddl_array_name(array) );
            dsim_ddl_array_type_clone( b, dsim_ddl_array_type(array) );
            dsim_ddl_layout_arrays_push_end( b );
        }
        dsim_ddl_layout_arrays_end( b );

        dsim_ddl_root_layouts_push_end( b );
    }
    dsim_ddl_root_layouts_end( b );
}

static void dsim_process_storages( struct flatcc_builder *b, dsim_ddl_storage_vec_t storages )
{
    dsim_ddl_root_storages_start( b );
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);

        dsim_ddl_root_storages_push_start( b );
        dsim_ddl_storage_name_clone( b, dsim_ddl_storage_name(storage) );
        dsim_ddl_storage_layout_clone( b, dsim_ddl_storage_layout(storage) );
        dsim_ddl_storage_type_add( b, dsim_ddl_storage_type(storage) );

        dsim_ddl_root_storages_push_end( b );
    }
    dsim_ddl_root_storages_end( b );
}

static void *dsim_process_ddl( const void *raw_data, const void *prev_data, size_t *size_out )
{
    dsim_ddl_type_vec_t prev_types = 0;
    if( prev_data )
    {
        dsim_ddl_root_table_t prev_ddl = dsim_ddl_root_as_root( prev_data );
        prev_types = dsim_ddl_root_types( prev_ddl );
    }

    struct flatcc_builder b;
    flatcc_builder_init( &b );
    dsim_ddl_root_start_as_root( &b );

    dsim_ddl_root_table_t root = dsim_ddl_root_as_root( raw_data );
    dsim_ddl_root_name_clone( &b, dsim_ddl_root_name(root) );

    dsim_process_types( &b, dsim_ddl_root_types(root), prev_types );
    dsim_process_layouts( &b, dsim_ddl_root_layouts(root) );
    dsim_process_storages( &b, dsim_ddl_root_storages(root) );

    dsim_ddl_root_end_as_root( &b );
    void *buf = flatcc_builder_finalize_buffer( &b, size_out );
    flatcc_builder_clear( &b );
    return buf;
}

void *dsim_ddl_compile_json( const char *json_data, size_t json_size, size_t *size_out )
{
    size_t raw_size;
    void *raw_data = dsim_ddl_load_json( json_data, json_size, &raw_size );
    if( !raw_data ) return 0;

    int pos = 0;
    void *buf[2] = { 0, 0 };
    size_t buf_size[2] = { 0, 0 };

    while( 1 )
    {
        buf[pos] = dsim_process_ddl( raw_data, buf[1-pos], buf_size + pos );
        if( (buf_size[0] == buf_size[1]) && (memcmp(buf[0], buf[1], buf_size[0]) == 0) )
        {
            free( raw_data );
            free( buf[1] );
            *size_out = buf_size[0];
            return buf[0];
        }

        pos = 1 - pos;
        if( buf[pos] ) free(buf[pos]);
    }
}

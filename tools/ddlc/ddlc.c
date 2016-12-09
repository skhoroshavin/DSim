
#include "reflection/ddl_reader.h"
#include "reflection/ddl_builder.h"
#include "reflection/ddl_json_parser.h"

#include "containers/array.h"
#include "containers/string.h"
#include "reflection/ddl_registry.h"
#include "utils/file.h"
#include "utils/log.h"
#include "ddl_json.h"

#include <stdio.h>

static struct dsim_array_ptr buffers = dsim_array_static_init(&dsim_default_allocator);

static void * load_json_ddl( const char * filename, size_t * size_out )
{
    struct dsim_array_char json = dsim_array_static_init( &dsim_default_allocator );
    dsim_read_file( filename, &json );

    void *buf = dsim_ddl_compile_json( json.data, json.count, size_out );
    dsim_array_char_reset( &json );
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

int main( int argc, char *argv[] )
{
    size_t size;
    void * data = load_json_ddl( argv[1], &size );
    if( !data )
        dsim_fatal( "Failed to load %s", argv[1] );
    dsim_ddl_root_table_t root = dsim_ddl_root_as_root( data );

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
    fwrite( data, 1, size, f );
    fclose( f );

    ///////////////////////////////////////////////////////////////////////////////
    // Header
    ///////////////////////////////////////////////////////////////////////////////

    f = fopen( h_name.data, "w" );
    fprintf( f, "\n" );
    fprintf( f, "#pragma once\n" );
    fprintf( f, "\n" );
    fprintf( f, "#include \"storage/storage.h\"\n" );
    fprintf( f, "#include \"reflection/ddl_reader.h\"\n" );
    fprintf( f, "\n" );

    dsim_ddl_type_vec_t types = dsim_ddl_root_types( root );
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        if( dsim_ddl_type_ctype_is_present(type) )
            continue;

        switch( dsim_ddl_type_data_type(type) )
        {
        case dsim_ddl_any_type_struct_type:
        {
            dsim_ddl_struct_type_table_t stype = (dsim_ddl_struct_type_table_t)dsim_ddl_type_data(type);
            dsim_ddl_struct_field_vec_t fields = dsim_ddl_struct_type_fields(stype);

            fprintf( f, "typedef struct %s {\n", dsim_ddl_type_name(type) );
            for( size_t j = 0; j < dsim_ddl_struct_field_vec_len(fields); ++j )
            {
                dsim_ddl_struct_field_table_t field = dsim_ddl_struct_field_vec_at(fields,j);
                fprintf( f, "    %s %s;\n", dsim_ddl_struct_field_type(field), dsim_ddl_struct_field_name(field) );
            }
            fprintf( f, "} %s;\n", dsim_ddl_type_name(type) );
            fprintf( f, "\n" );
            break;
        }
        case dsim_ddl_any_type_enum_type:
        {
            dsim_ddl_enum_type_table_t etype = (dsim_ddl_enum_type_table_t)dsim_ddl_type_data(type);
            flatbuffers_string_vec_t values = dsim_ddl_enum_type_values(etype);
            size_t values_count = flatbuffers_string_vec_len(values);

            fprintf( f, "typedef enum %s {\n", dsim_ddl_type_name(type) );
            for( size_t j = 0; j < values_count; ++j )
            {
                flatbuffers_string_t value = flatbuffers_string_vec_at(values,j);
                fprintf( f, "    %s_%s", dsim_ddl_type_name(type), value );
                if( j < values_count-1 )
                    fprintf( f, "," );
                fprintf( f, "\n" );
            }
            fprintf( f, "} %s;\n", dsim_ddl_type_name(type) );
            fprintf( f, "\n" );

            break;
        }
        case dsim_ddl_any_type_reference_type:
        {
            //dsim_ddl_reference_type_table_t rtype = (dsim_ddl_reference_type_table_t)dsim_ddl_type_data(type);

            fprintf( f, "typedef struct %s { uint64_t id; } %s;\n", dsim_ddl_type_name(type), dsim_ddl_type_name(type) );
            fprintf( f, "\n" );

            break;
        }
        }
    }

    dsim_ddl_layout_vec_t layouts = dsim_ddl_root_layouts( root );
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);

        const char * name = dsim_ddl_layout_name(layout);
        dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(layout);
        for( size_t j = 0; j != dsim_ddl_array_vec_len(arrays); ++j )
        {
            dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays, j);
            dsim_ddl_type_table_t type = dsim_ddl_types_scan_by_name( types, dsim_ddl_array_type(array) );

            const char * ctype = dsim_ddl_type_ctype(type);
            if( !ctype ) ctype = dsim_ddl_type_name(type);

            fprintf( f, "inline static %s *%s_%s_data( struct dsim_storage *s, uint32_t block ) { return (%s *)dsim_storage_data( s, block, %zd ); }\n",
                     ctype, name, dsim_ddl_array_name(array), ctype, j );
        }
        fprintf( f, "\n" );
    }

    fprintf( f, "struct _ddl_%s\n", dsim_ddl_root_name(root) );
    fprintf( f, "{\n" );
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        fprintf( f, "    dsim_ddl_type_table_t type_%s;\n", dsim_ddl_type_name(type) );
    }
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);
        fprintf( f, "    dsim_ddl_layout_table_t layout_%s;\n", dsim_ddl_layout_name(layout) );
    }
    fprintf( f, "};\n" );
    fprintf( f, "extern const struct _ddl_%s *const ddl_%s;\n", dsim_ddl_root_name(root), dsim_ddl_root_name(root) );
    fprintf( f, "\n" );

    dsim_ddl_storage_vec_t storages = dsim_ddl_root_storages( root );
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        fprintf( f, "extern struct dsim_storage * %s;\n", dsim_ddl_storage_name(storage) );
    }
    if( dsim_ddl_storage_vec_len(storages) )
        fprintf( f, "\n" );

    fprintf( f, "void dsim_ddl_init_%s();\n", dsim_ddl_root_name(root) );
    fprintf( f, "void dsim_ddl_done_%s();\n", dsim_ddl_root_name(root) );
    fprintf( f, "\n" );
    fclose( f );

    ///////////////////////////////////////////////////////////////////////////////
    // Source
    ///////////////////////////////////////////////////////////////////////////////

    f = fopen( c_name.data, "w" );
    fprintf( f, "\n" );
    fprintf( f, "#include \"reflection/ddl_registry.h\"\n" );
    fprintf( f, "#include \"storage/hash_storage.h\"\n" );
    fprintf( f, "#include \"%s\"\n", h_name.data );
    fprintf( f, "\n" );
    fprintf( f, "static const char dsim_ddl_%s_data[] = {\n", dsim_ddl_root_name(root) );
    dump_c_array( f, (const char *)data, size );
    fprintf( f, "};\n" );
    fprintf( f, "\n" );

    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        switch( dsim_ddl_storage_type(storage) )
        {
        case dsim_ddl_storage_type_hashed:
            fprintf( f, "static struct dsim_hash_storage strg_%s;\n", dsim_ddl_storage_name(storage) );
            break;
        }

        fprintf( f, "struct dsim_storage * %s = &strg_%s.storage;\n", dsim_ddl_storage_name(storage), dsim_ddl_storage_name(storage) );
        fprintf( f, "\n" );
    }

    fprintf( f, "static struct _ddl_%s _ddl;\n", dsim_ddl_root_name(root) );
    fprintf( f, "const struct _ddl_%s *const ddl_%s = &_ddl;\n", dsim_ddl_root_name(root), dsim_ddl_root_name(root) );
    fprintf( f, "\n" );

    fprintf( f, "void dsim_ddl_init_%s()\n", dsim_ddl_root_name(root) );
    fprintf( f, "{\n" );
    fprintf( f, "    dsim_ddl_register( dsim_ddl_%s_data );\n", dsim_ddl_root_name(root) );
    fprintf( f, "\n" );
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        fprintf( f, "    _ddl.type_%s = dsim_ddl_type( \"%s\" );\n", dsim_ddl_type_name(type), dsim_ddl_type_name(type) );
    }
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);
        fprintf( f, "    _ddl.layout_%s = dsim_ddl_layout( \"%s\" );\n", dsim_ddl_layout_name(layout), dsim_ddl_layout_name(layout) );
    }
    fprintf( f, "\n" );
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        switch( dsim_ddl_storage_type(storage) )
        {
        case dsim_ddl_storage_type_hashed:
            fprintf( f, "    dsim_hash_storage_init( &strg_%s, _ddl.layout_%s, &dsim_default_allocator );\n",
                     dsim_ddl_storage_name(storage), dsim_ddl_storage_layout(storage) );
            break;
        }
    }
    fprintf( f, "}\n" );

    fprintf( f, "void dsim_ddl_done_%s()\n", dsim_ddl_root_name(root) );
    fprintf( f, "{\n" );
    fprintf( f, "    dsim_ddl_registry_reset();\n" );
    fprintf( f, "\n" );
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        fprintf( f, "    dsim_storage_done( %s );\n", dsim_ddl_storage_name(storage) );
    }
    fprintf( f, "}\n" );

    fclose( f );

    dsim_string_reset( &bin_name );
    dsim_string_reset( &h_name );
    dsim_string_reset( &c_name );

    free( data );

    dsim_ddl_registry_reset();
    for( uint32_t i = 0; i < buffers.count; ++i )
        free( buffers.at[i] );
    dsim_array_ptr_reset( &buffers );

    return 0;
}

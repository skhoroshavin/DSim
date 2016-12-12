
#include "ddl_c.h"

#include <stdio.h>

#include "reflection/ddl_utils.h"
#include "containers/string.h"

static void _dsim_ddl_generate_h( const char *h_name, void *data )
{
    FILE *f = fopen( h_name, "w" );

    dsim_ddl_root_table_t root = dsim_ddl_root_as_root( data );
    const char *root_name = dsim_ddl_root_name( root );

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

    fprintf( f, "struct _ddl_%s\n", root_name );
    fprintf( f, "{\n" );
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        fprintf( f, "    dsim_ddl_type_table_t type_%s;\n", dsim_ddl_type_name(type) );
    }
    dsim_ddl_layout_vec_t layouts = dsim_ddl_root_layouts( root );
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);
        fprintf( f, "    dsim_ddl_layout_table_t layout_%s;\n", dsim_ddl_layout_name(layout) );
    }
    dsim_ddl_storage_vec_t storages = dsim_ddl_root_storages( root );
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        fprintf( f, "    struct dsim_storage *storage_%s;\n", dsim_ddl_storage_name(storage) );
    }
    fprintf( f, "};\n" );
    fprintf( f, "extern const struct _ddl_%s *const ddl_%s;\n", root_name, root_name );
    fprintf( f, "\n" );

    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        const char *storage_name = dsim_ddl_storage_name(storage);

        dsim_ddl_layout_table_t layout = dsim_ddl_layout_by_name( root, dsim_ddl_storage_layout(storage) );
        dsim_ddl_array_vec_t arrays = dsim_ddl_layout_arrays(layout);

        fprintf( f, "inline static void %s_%s_insert( const uint64_t *ids", root_name, storage_name );
        for( size_t j = 0; j != dsim_ddl_array_vec_len(arrays); ++j )
        {
            dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays, j);
            const char *array_name = dsim_ddl_array_name(array);

            dsim_ddl_type_table_t type = dsim_ddl_type_by_name( root, dsim_ddl_array_type(array) );
            const char *ctype = dsim_ddl_type_ctype(type);
            if( !ctype ) ctype = dsim_ddl_type_name(type);

            fprintf( f, ", const %s *%s", ctype, array_name );
        }
        fprintf( f, ", uint32_t count )\n{ const void *_data[] = { " );
        for( size_t j = 0; j != dsim_ddl_array_vec_len(arrays); ++j )
        {
            dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays, j);
            const char *array_name = dsim_ddl_array_name(array);
            if( j > 0 ) fprintf( f, ", " );
            fprintf( f, "%s", array_name );
        }
        fprintf( f, " }; dsim_storage_insert( ddl_%s->storage_%s, ids, _data, count ); }\n", root_name, storage_name );

        fprintf( f, "inline static void %s_%s_remove( const uint64_t *ids, uint32_t count )\n", root_name, storage_name );
        fprintf( f, "{ dsim_storage_remove( ddl_%s->storage_%s, ids, count ); }\n", root_name, storage_name );

        fprintf( f, "inline static dsim_storage_index %s_%s_find( uint64_t id )\n", root_name, storage_name );
        fprintf( f, "{ return dsim_storage_find( ddl_%s->storage_%s, id ); }\n", root_name, storage_name );

        fprintf( f, "inline static const uint64_t *%s_%s_id_data( uint32_t block )"
                    "{ return (const uint64_t *)dsim_storage_id_data( ddl_%s->storage_%s, block ); }\n",
                 root_name, storage_name, root_name, storage_name );

        for( size_t j = 0; j != dsim_ddl_array_vec_len(arrays); ++j )
        {
            dsim_ddl_array_table_t array = dsim_ddl_array_vec_at(arrays, j);
            const char *array_name = dsim_ddl_array_name(array);

            dsim_ddl_type_table_t type = dsim_ddl_type_by_name( root, dsim_ddl_array_type(array) );
            const char *ctype = dsim_ddl_type_ctype(type);
            if( !ctype ) ctype = dsim_ddl_type_name(type);

            fprintf( f, "inline static const %s *%s_%s_%s_data( uint32_t block )"
                        "{ return (const %s *)dsim_storage_data( ddl_%s->storage_%s, block, %zd ); }\n",
                     ctype, root_name, storage_name, array_name, ctype, root_name, storage_name, j );
        }
    }
    fprintf( f, "\n" );

    fprintf( f, "void dsim_ddl_init_%s();\n", root_name );
    fprintf( f, "void dsim_ddl_done_%s();\n", root_name );
    fprintf( f, "\n" );
    fclose( f );
}

static void dump_c_array( FILE *f, const char *data, size_t size )
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

static void _dsim_ddl_generate_c( const char *h_name, const char *c_name, void *data, size_t size )
{
    FILE *f = fopen( c_name, "w" );

    dsim_ddl_root_table_t root = dsim_ddl_root_as_root( data );
    const char *root_name = dsim_ddl_root_name( root );

    fprintf( f, "\n" );
    fprintf( f, "#include \"reflection/ddl_registry.h\"\n" );
    fprintf( f, "#include \"storage/hash_storage.h\"\n" );
    fprintf( f, "#include \"%s\"\n", h_name);
    fprintf( f, "\n" );
    fprintf( f, "static const char dsim_ddl_%s_data[] = {\n", root_name );
    dump_c_array( f, (const char *)data, size );
    fprintf( f, "};\n" );
    fprintf( f, "\n" );

    fprintf( f, "static struct _ddl_%s _ddl;\n", root_name );
    fprintf( f, "const struct _ddl_%s *const ddl_%s = &_ddl;\n", root_name, root_name );
    fprintf( f, "\n" );

    fprintf( f, "void dsim_ddl_init_%s()\n", root_name );
    fprintf( f, "{\n" );
    fprintf( f, "    dsim_ddl_register( dsim_ddl_%s_data );\n", root_name );
    fprintf( f, "\n" );
    dsim_ddl_type_vec_t types = dsim_ddl_root_types(root);
    for( size_t i = 0; i != dsim_ddl_type_vec_len(types); ++i )
    {
        dsim_ddl_type_table_t type = dsim_ddl_type_vec_at(types, i);
        fprintf( f, "    _ddl.type_%s = dsim_ddl_type( \"%s\" );\n", dsim_ddl_type_name(type), dsim_ddl_type_name(type) );
    }
    dsim_ddl_layout_vec_t layouts = dsim_ddl_root_layouts(root);
    for( size_t i = 0; i != dsim_ddl_layout_vec_len(layouts); ++i )
    {
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_vec_at(layouts, i);
        fprintf( f, "    _ddl.layout_%s = dsim_ddl_layout( \"%s\" );\n", dsim_ddl_layout_name(layout), dsim_ddl_layout_name(layout) );
    }
    dsim_ddl_storage_vec_t storages = dsim_ddl_root_storages(root);
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        fprintf( f, "    _ddl.storage_%s = dsim_ddl_storage( \"%s\" );\n", dsim_ddl_storage_name(storage), dsim_ddl_storage_name(storage) );
    }
    fprintf( f, "}\n\n" );

    fprintf( f, "void dsim_ddl_done_%s()\n", root_name );
    fprintf( f, "{\n" );
    fprintf( f, "    dsim_ddl_registry_reset();\n" );
    fprintf( f, "}\n\n" );

    fclose( f );
}

void dsim_ddl_generate_c( const char *base_name, void *data, size_t size )
{
    struct dsim_string h_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &h_name, base_name );
    dsim_string_append( &h_name, ".h" );

    struct dsim_string c_name = dsim_string_static_init(&dsim_default_allocator);
    dsim_string_append( &c_name, base_name );
    dsim_string_append( &c_name, ".c" );

    _dsim_ddl_generate_h( h_name.data, data );
    _dsim_ddl_generate_c( h_name.data, c_name.data, data, size );

    dsim_string_reset( &h_name );
    dsim_string_reset( &c_name );
}

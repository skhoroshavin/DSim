
#include "ddl_registry.h"
#include "ddl_utils.h"
#include "containers/array.h"
#include "utils/log.h"
#include "storage/hash_storage.h"

static struct dsim_array_cptr ddl_roots = dsim_array_static_init(&dsim_default_allocator);

DSIM_DEFINE_ARRAY(struct dsim_storage*,storage_ptr)
static struct dsim_array_storage_ptr ddl_storages = dsim_array_static_init(&dsim_default_allocator);

dsim_ddl_type_table_t dsim_ddl_type( const char *name )
{
    for( uint32_t i = 0; i != ddl_roots.count; ++i )
    {
        dsim_ddl_root_table_t root = dsim_ddl_root_as_root(ddl_roots.at[i]);
        dsim_ddl_type_table_t type = dsim_ddl_type_by_name( root, name );
        if( type ) return type;
    }
    return 0;
}

dsim_ddl_layout_table_t dsim_ddl_layout( const char *name )
{
    for( uint32_t i = 0; i != ddl_roots.count; ++i )
    {
        dsim_ddl_root_table_t root = dsim_ddl_root_as_root(ddl_roots.at[i]);
        dsim_ddl_layout_table_t layout = dsim_ddl_layout_by_name( root, name );
        if( layout ) return layout;
    }
    return 0;
}

struct dsim_storage * dsim_ddl_storage( const char *name )
{
    for( uint32_t i = 0; i != ddl_storages.count; ++i )
    {
        struct dsim_storage * storage = ddl_storages.at[i];
        if( strcmp( storage->name, name ) == 0 )
            return storage;
    }
    return 0;
}

void dsim_ddl_register( const void * data )
{
    dsim_array_cptr_push_back( &ddl_roots, data );

    dsim_ddl_root_table_t root = dsim_ddl_root_as_root(data);
    dsim_ddl_hash_storage_vec_t storages = dsim_ddl_root_storages(root);
    for( size_t i = 0; i != dsim_ddl_storage_vec_len(storages); ++i )
    {
        dsim_ddl_storage_table_t storage = dsim_ddl_storage_vec_at(storages, i);
        const char *name = dsim_ddl_storage_name(storage);
        dsim_ddl_layout_table_t layout = dsim_ddl_layout( dsim_ddl_storage_layout(storage) );
        dsim_ddl_storage_engine_union_type_t type = dsim_ddl_storage_engine_type(storage);

        switch( type )
        {
        case dsim_ddl_storage_engine_hash_storage:
        {
            struct dsim_hash_storage * hash = (struct dsim_hash_storage*)dsim_allocate( &dsim_default_allocator, sizeof(struct dsim_hash_storage) );
            dsim_hash_storage_init( hash, name, layout, &dsim_default_allocator );
            dsim_array_storage_ptr_push_back( &ddl_storages, &hash->storage );
            break;
        }
        default:
            dsim_fatal( "Unsupported storage engine %s\n", dsim_ddl_storage_engine_type_name(type) );
        }
    }
}

void dsim_ddl_registry_init()
{

}

void dsim_ddl_registry_reset()
{
    dsim_array_cptr_reset( &ddl_roots );

    for( uint32_t i = 0; i < ddl_storages.count; ++i )
    {
        dsim_storage_done( ddl_storages.at[i] );
        dsim_deallocate( &dsim_default_allocator, ddl_storages.at[i] );
    }
    dsim_array_storage_ptr_reset( &ddl_storages );
}


#pragma once

#include "storage_log.h"
#include "reflection/ddl_reader.h"

DSIM_BEGIN_HEADER

typedef struct dsim_storage_index
{
    uint32_t block;
    uint32_t index;
} dsim_storage_index;

typedef struct dsim_storage_range
{
    uint32_t block;
    uint32_t start_index;
    uint32_t count;
} dsim_storage_range;

DSIM_DEFINE_ARRAY(dsim_storage_range, storage_range) // LCOV_EXCL_BR_LINE

struct dsim_storage;

struct dsim_storage_operations
{
    uint32_t (*block_count)( const struct dsim_storage *self );
    uint32_t (*block_size)( const struct dsim_storage *self, uint32_t block );

    const uint64_t * (*id_data)( const struct dsim_storage *self, uint32_t block );
    void *           (*data)( struct dsim_storage *self, uint32_t block, uint32_t i );

    dsim_storage_index (*find)( const struct dsim_storage *self, uint64_t id );
    void             (*find_range)( const struct dsim_storage *self, uint64_t start_id, uint32_t count, struct dsim_array_storage_range * result );

    void     (*insert)( struct dsim_storage *self, uint64_t start_id, uint32_t count );
    void     (*remove)( struct dsim_storage *self, uint64_t start_id, uint32_t count );
    void     (*done)( struct dsim_storage *self );
};

struct dsim_storage
{
    const struct dsim_storage_operations * _ops;
    struct dsim_storage_log log;
    const char * name;
    dsim_ddl_layout_table_t layout;
};

inline static void dsim_storage_init( struct dsim_storage *storage, const struct dsim_storage_operations *ops,
                                      const char * name, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc )
{
    storage->_ops = ops;
    storage->name = name;
    storage->layout = layout;
    dsim_storage_log_init( &storage->log, alloc );
}

inline static uint32_t dsim_storage_array_count( const struct dsim_storage *storage )
{ return dsim_ddl_array_vec_len( dsim_ddl_layout_arrays(storage->layout) ); }

inline static uint32_t dsim_storage_block_count( const struct dsim_storage *storage )
{ return storage->_ops->block_count( storage ); }
inline static uint32_t dsim_storage_block_size( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->block_size( storage, block ); }

inline static const uint64_t *dsim_storage_id_data( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->id_data( storage, block ); }
inline static void *dsim_storage_data( struct dsim_storage *storage, uint32_t block, uint32_t i )
{ return storage->_ops->data( storage, block, i ); }

inline static dsim_storage_index dsim_storage_find( const struct dsim_storage *storage, uint64_t id )
{ return storage->_ops->find( storage, id ); }
inline static void dsim_storage_find_range( const struct dsim_storage *storage, uint64_t start_id, uint32_t count, struct dsim_array_storage_range * result )
{ storage->_ops->find_range( storage, start_id, count, result ); }
inline static void dsim_storage_insert( struct dsim_storage *storage, uint64_t start_id, uint32_t count )
{ storage->_ops->insert( storage, start_id, count ); }
inline static void dsim_storage_remove( struct dsim_storage *storage, uint64_t start_id, uint32_t count )
{ storage->_ops->remove( storage, start_id, count ); }
inline static void dsim_storage_done( struct dsim_storage *storage )
{ storage->_ops->done( storage ); dsim_storage_log_reset( &storage->log ); }

DSIM_END_HEADER

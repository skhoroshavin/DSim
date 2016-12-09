
#pragma once

#include "storage_log.h"
#include "reflection.h"

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
    uint32_t       (*array_count)( const struct dsim_storage *self );
    uint32_t       (*array_size)( const struct dsim_storage *self, uint32_t col );
    const char    *(*array_name)( const struct dsim_storage *self, uint32_t col );
    enum dsim_type (*array_type)( const struct dsim_storage *self, uint32_t col );

    uint32_t (*block_count)( const struct dsim_storage *self );
    uint32_t (*block_size)( const struct dsim_storage *self, uint32_t block );

    const uint64_t * (*id_data)( const struct dsim_storage *self, uint32_t block );
    void *           (*data)( struct dsim_storage *self, uint32_t block, uint32_t col );

    dsim_storage_index (*find)( const struct dsim_storage *self, uint64_t id );
    void             (*find_range)( const struct dsim_storage *self, uint64_t start_id, uint32_t count, struct dsim_array_storage_range * result );

    void     (*insert)( struct dsim_storage *self, uint64_t start_id, uint32_t count );
    void     (*remove)( struct dsim_storage *self, uint64_t start_id, uint32_t count );
    void     (*reset)( struct dsim_storage *self );
};

struct dsim_storage
{
    struct dsim_storage_operations * _ops;
    struct dsim_storage_log log;
};

#define dsim_storage_static_init(ops,alloc) { \
    /* ._ops = */ ops, \
    /* .log = */ dsim_storage_log_static_init(alloc) }

inline static uint32_t dsim_storage_array_count( const struct dsim_storage *storage )
{ return storage->_ops->array_count( storage ); }
inline static uint32_t dsim_storage_array_size( const struct dsim_storage *storage, uint32_t col )
{ return storage->_ops->array_size( storage, col ); }
inline static const char * dsim_storage_array_name( const struct dsim_storage *storage, uint32_t col )
{ return storage->_ops->array_name( storage, col ); }
inline static enum dsim_type dsim_storage_array_type( const struct dsim_storage *storage, uint32_t col )
{ return storage->_ops->array_type( storage, col ); }

inline static uint32_t dsim_storage_block_count( const struct dsim_storage *storage )
{ return storage->_ops->block_count( storage ); }
inline static uint32_t dsim_storage_block_size( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->block_size( storage, block ); }

inline static const uint64_t *dsim_storage_id_data( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->id_data( storage, block ); }
inline static void *dsim_storage_data( struct dsim_storage *storage, uint32_t block, uint32_t col )
{ return storage->_ops->data( storage, block, col ); }

inline static dsim_storage_index dsim_storage_find( const struct dsim_storage *storage, uint64_t id )
{ return storage->_ops->find( storage, id ); }
inline static void dsim_storage_find_range( const struct dsim_storage *storage, uint64_t start_id, uint32_t count, struct dsim_array_storage_range * result )
{ storage->_ops->find_range( storage, start_id, count, result ); }
inline static void dsim_storage_insert( struct dsim_storage *storage, uint64_t start_id, uint32_t count )
{ storage->_ops->insert( storage, start_id, count ); }
inline static void dsim_storage_remove( struct dsim_storage *storage, uint64_t start_id, uint32_t count )
{ storage->_ops->remove( storage, start_id, count ); }
inline static void dsim_storage_reset( struct dsim_storage *storage )
{ storage->_ops->reset( storage ); dsim_storage_log_reset( &storage->log ); }

DSIM_END_HEADER

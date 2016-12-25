
#pragma once

#include "storage_log.h"
#include "storage_array.h"
#include "reflection/ddl_reader.h"

DSIM_BEGIN_HEADER

typedef void (*dsim_storage_select_cb)( void *context, uint32_t pos, uint32_t block, uint32_t block_pos, uint32_t count );

struct dsim_storage;

struct dsim_storage_operations
{
    uint32_t (*block_count)( const struct dsim_storage *self );
    uint32_t (*block_size)( const struct dsim_storage *self, uint32_t block );

    const uint64_t *(*id_data)( const struct dsim_storage *self, uint32_t block );

    const void *(*begin_read)( struct dsim_storage *self, uint32_t block, uint32_t arr );
    unsigned    (*end_read)( struct dsim_storage *self, const void *data );
    void       *(*begin_write)( struct dsim_storage *self, uint32_t block, uint32_t arr, enum dsim_storage_write_mode mode );
    unsigned    (*end_write)( struct dsim_storage *self, void *data );

    int  (*can_modify)( const struct dsim_storage *self, uint32_t block, uint32_t arr );
    void (*select)( struct dsim_storage *self, const uint64_t *ids, uint32_t count, dsim_storage_select_cb cb, void *context );
    void (*insert)( struct dsim_storage *self, const uint64_t *ids, const void *const *data, uint32_t count );
    void (*update)( struct dsim_storage *self, const uint64_t *ids, const void *const *data, uint32_t count );
    void (*remove)( struct dsim_storage *self, const uint64_t *ids, uint32_t count );

    void (*done)( struct dsim_storage *self );
};

struct dsim_storage
{
    const struct dsim_storage_operations *_ops;
    struct dsim_storage_log log;
    const char *name;
    dsim_ddl_layout_table_t layout;
};

inline static void dsim_storage_init( struct dsim_storage *storage, const struct dsim_storage_operations *ops,
                                      const char *name, dsim_ddl_layout_table_t layout, struct dsim_allocator *alloc )
{
    storage->_ops = ops;
    storage->name = name;
    storage->layout = layout;
    dsim_storage_log_init( &storage->log, alloc );
}

inline static uint32_t dsim_storage_block_count( const struct dsim_storage *storage )
{ return storage->_ops->block_count( storage ); }
inline static uint32_t dsim_storage_block_size( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->block_size( storage, block ); }

inline static const uint64_t *dsim_storage_id_data( const struct dsim_storage *storage, uint32_t block )
{ return storage->_ops->id_data( storage, block ); }
inline static const void *dsim_storage_begin_read( struct dsim_storage *storage, uint32_t block, uint32_t arr )
{ return storage->_ops->begin_read( storage, block, arr ); }
inline static unsigned dsim_storage_end_read( struct dsim_storage *storage, const void *data )
{ return storage->_ops->end_read( storage, data ); }
inline static void *dsim_storage_begin_write( struct dsim_storage *storage, uint32_t block, uint32_t arr, enum dsim_storage_write_mode mode )
{ return storage->_ops->begin_write( storage, block, arr, mode ); }
inline static unsigned dsim_storage_end_write( struct dsim_storage *storage, void *data )
{ return storage->_ops->end_write( storage, data ); }

inline static int dsim_storage_can_modify( const struct dsim_storage *storage, uint32_t block, uint32_t arr )
{ return storage->_ops->can_modify( storage, block, arr ); }
inline static void dsim_storage_select( struct dsim_storage *storage, const uint64_t *ids, uint32_t count, dsim_storage_select_cb cb, void *context )
{ storage->_ops->select( storage, ids, count, cb, context ); }
inline static void dsim_storage_insert( struct dsim_storage *storage, const uint64_t *ids, const void *const *data, uint32_t count )
{ storage->_ops->insert( storage, ids, data, count ); }
inline static void dsim_storage_update( struct dsim_storage *storage, const uint64_t *ids, const void *const *data, uint32_t count )
{ storage->_ops->update( storage, ids, data, count ); }
inline static void dsim_storage_remove( struct dsim_storage *storage, const uint64_t *ids, uint32_t count )
{ storage->_ops->remove( storage, ids, count ); }

inline static void dsim_storage_done( struct dsim_storage *storage )
{ storage->_ops->done( storage ); dsim_storage_log_reset( &storage->log ); }

typedef struct dsim_storage_addr
{
    uint32_t block;
    uint32_t index;
} dsim_storage_addr;

void dsim_storage_select_buf( struct dsim_storage *storage, const uint64_t *ids, dsim_storage_addr *addr, uint32_t count );

DSIM_END_HEADER

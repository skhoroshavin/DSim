
#pragma once

#include "table_log.h"

DSIM_BEGIN_HEADER

typedef struct dsim_table_index
{
    uint32_t block;
    uint32_t index;
} dsim_table_index;

typedef struct dsim_table_range
{
    uint32_t block;
    uint32_t start_index;
    uint32_t count;
} dsim_table_range;

DSIM_DEFINE_ARRAY(dsim_table_range, table_range) // LCOV_EXCL_BR_LINE

struct dsim_table;

struct dsim_table_operations
{
    uint32_t (*column_count)( const struct dsim_table *self );
    uint32_t (*column_size)( const struct dsim_table *self, uint32_t col );
    uint32_t (*block_count)( const struct dsim_table *self );
    uint32_t (*block_size)( const struct dsim_table *self, uint32_t block );

    const uint64_t * (*id_data)( const struct dsim_table *self, uint32_t block );
    void *           (*data)( struct dsim_table *self, uint32_t block, uint32_t col );

    dsim_table_index (*find)( const struct dsim_table *self, uint64_t id );
    void             (*find_range)( const struct dsim_table *self, uint64_t start_id, uint32_t count, struct dsim_array_table_range * result );

    void     (*insert)( struct dsim_table *self, uint64_t start_id, uint32_t count );
    void     (*remove)( struct dsim_table *self, uint64_t start_id, uint32_t count );
    void     (*reset)( struct dsim_table *self );
};

struct dsim_table
{
    struct dsim_table_operations * _ops;
    struct dsim_table_log log;
};

#define dsim_table_static_init(ops,alloc) { \
    /* ._ops = */ ops, \
    /* .log = */ dsim_table_log_static_init(alloc) }

inline static uint32_t dsim_table_column_count( const struct dsim_table *table )
{ return table->_ops->column_count( table ); }
inline static uint32_t dsim_table_column_size( const struct dsim_table *table, uint32_t col )
{ return table->_ops->column_size( table, col ); }
inline static uint32_t dsim_table_block_count( const struct dsim_table *table )
{ return table->_ops->block_count( table ); }
inline static uint32_t dsim_table_block_size( const struct dsim_table *table, uint32_t block )
{ return table->_ops->block_size( table, block ); }

inline static const uint64_t *dsim_table_id_data( const struct dsim_table *table, uint32_t block )
{ return table->_ops->id_data( table, block ); }
inline static void *dsim_table_data( struct dsim_table *table, uint32_t block, uint32_t col )
{ return table->_ops->data( table, block, col ); }

inline static dsim_table_index dsim_table_find( const struct dsim_table *table, uint64_t id )
{ return table->_ops->find( table, id ); }
inline static void dsim_table_find_range( const struct dsim_table *table, uint64_t start_id, uint32_t count, struct dsim_array_table_range * result )
{ table->_ops->find_range( table, start_id, count, result ); }
inline static void dsim_table_insert( struct dsim_table *table, uint64_t start_id, uint32_t count )
{ table->_ops->insert( table, start_id, count ); }
inline static void dsim_table_remove( struct dsim_table *table, uint64_t start_id, uint32_t count )
{ table->_ops->remove( table, start_id, count ); }
inline static void dsim_table_reset( struct dsim_table *table )
{ table->_ops->reset( table ); dsim_table_log_reset( &table->log ); }

DSIM_END_HEADER

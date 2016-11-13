
#pragma once

#include "utils/common.h"

struct dsim_table;

struct dsim_table_operations
{
    uint32_t (*column_count)( const struct dsim_table *self );
    uint32_t (*column_size)( const struct dsim_table *self, uint32_t col );
    uint32_t (*block_count)( const struct dsim_table *self );
    uint32_t (*block_size)( const struct dsim_table *self, uint32_t block );

    const uint64_t * (*id_data)( const struct dsim_table *self, uint32_t block );
    void *           (*data)( struct dsim_table *self, uint32_t block, uint32_t col );

    void (*insert)( struct dsim_table *self, uint64_t start_id, uint32_t count );
    void (*remove)( struct dsim_table *self, uint64_t start_id, uint32_t count );
};

struct dsim_table
{
    struct dsim_table_operations * _ops;
};

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

inline static void dsim_insert( struct dsim_table *table, uint64_t start_id, uint32_t count )
{ table->_ops->insert( table, start_id, count ); }
inline static void dsim_remove( struct dsim_table *table, uint64_t start_id, uint32_t count )
{ table->_ops->remove( table, start_id, count ); }

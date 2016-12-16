
#pragma once

#include "containers/array.h"
#include "reflection/ddl_registry.h"

struct dsim_storage_array
{
    struct _dsim_array current;
    struct _dsim_array next;
    dsim_ddl_type_table_t type;
    uint32_t read_count;
    uint32_t write_direct;
    uint32_t write_buffered;
};


void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc );
int dsim_storage_array_can_modify( const struct dsim_storage_array *sa );
void dsim_storage_array_resize( struct dsim_storage_array *sa, uint32_t count );
void dsim_storage_array_push_back( struct dsim_storage_array *sa, const void *data, uint32_t count );
void dsim_storage_array_update( struct dsim_storage_array *sa, const void *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count );
void dsim_storage_array_remove_fast( struct dsim_storage_array *sa, uint32_t pos, uint32_t count );
void dsim_storage_array_reset( struct dsim_storage_array *sa );

const void *dsim_storage_array_read_begin( struct dsim_storage_array *sa );
void dsim_storage_array_read_end( struct dsim_storage_array *sa );
void *dsim_storage_array_write_direct_begin( struct dsim_storage_array *sa );
void dsim_storage_array_write_direct_end( struct dsim_storage_array *sa );
void *dsim_storage_array_write_buffered_begin( struct dsim_storage_array *sa );
void dsim_storage_array_write_buffered_end( struct dsim_storage_array *sa );

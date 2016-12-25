
#pragma once

#include "containers/array.h"
#include "reflection/ddl_registry.h"

enum dsim_storage_write_mode
{
    DSIM_WRITE_NONE,
    DSIM_WRITE_DIRECT,
    DSIM_WRITE_BUFFERED
};

struct dsim_storage_array
{
    struct _dsim_array current;
    struct _dsim_array next;
    dsim_ddl_type_table_t type;
    uint32_t current_read_count;
    uint32_t next_read_count;
    enum dsim_storage_write_mode write_mode;
};


void dsim_storage_array_init( struct dsim_storage_array *sa, dsim_ddl_type_table_t type, struct dsim_allocator *alloc );
int dsim_storage_array_can_modify( const struct dsim_storage_array *sa );
void dsim_storage_array_resize( struct dsim_storage_array *sa, uint32_t count );
void dsim_storage_array_push_back( struct dsim_storage_array *sa, const void *data, uint32_t count );
void dsim_storage_array_update( struct dsim_storage_array *sa, const void *data, uint32_t src_offset, uint32_t dst_offset, uint32_t count );
void dsim_storage_array_remove_fast( struct dsim_storage_array *sa, uint32_t pos, uint32_t count );
void dsim_storage_array_reset( struct dsim_storage_array *sa );

const void *dsim_storage_array_begin_read( struct dsim_storage_array *sa );
int dsim_storage_array_end_read( struct dsim_storage_array *sa, const void *data );
void *dsim_storage_array_begin_write( struct dsim_storage_array *sa, enum dsim_storage_write_mode mode );
int dsim_storage_array_end_write( struct dsim_storage_array *sa, void *data );

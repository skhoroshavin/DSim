
#pragma once

#include "storage/storage.h"
#include "reflection/ddl_reader.h"

typedef struct vec2 {
    float x;
    float y;
} vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct wire_ref { uint64_t id; } wire_ref;

typedef enum logic_type {
    logic_type_and,
    logic_type_or,
    logic_type_nand,
    logic_type_nor,
    logic_type_xor
} logic_type;

struct _ddl_sample
{
    dsim_ddl_type_table_t type_uint8;
    dsim_ddl_type_table_t type_uint16;
    dsim_ddl_type_table_t type_uint32;
    dsim_ddl_type_table_t type_uint64;
    dsim_ddl_type_table_t type_float;
    dsim_ddl_type_table_t type_double;
    dsim_ddl_type_table_t type_vec2;
    dsim_ddl_type_table_t type_vec3;
    dsim_ddl_type_table_t type_vec4;
    dsim_ddl_type_table_t type_wire_ref;
    dsim_ddl_type_table_t type_logic_type;
    dsim_ddl_layout_table_t layout_wire;
    dsim_ddl_layout_table_t layout_logic;
    dsim_ddl_layout_table_t layout_delay;
    struct dsim_storage *storage_wires;
    struct dsim_storage *storage_logic;
    struct dsim_storage *storage_delay;
};
extern const struct _ddl_sample *const ddl_sample;

inline static void sample_wires_insert( const uint64_t *ids, const uint8_t *state, uint32_t count )
{ const void *_data[] = { state }; dsim_storage_insert( ddl_sample->storage_wires, ids, _data, count ); }
inline static void sample_wires_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_wires, ids, count ); }
inline static dsim_storage_index sample_wires_find( uint64_t id )
{ return dsim_storage_find( ddl_sample->storage_wires, id ); }
inline static const uint64_t *sample_wires_id_data( uint32_t block ){ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_wires, block ); }
inline static const uint8_t *sample_wires_state_data( uint32_t block ){ return (const uint8_t *)dsim_storage_data( ddl_sample->storage_wires, block, 0 ); }
inline static void sample_logic_insert( const uint64_t *ids, const wire_ref *input_1, const wire_ref *input_2, const wire_ref *output, const logic_type *type, uint32_t count )
{ const void *_data[] = { input_1, input_2, output, type }; dsim_storage_insert( ddl_sample->storage_logic, ids, _data, count ); }
inline static void sample_logic_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_logic, ids, count ); }
inline static dsim_storage_index sample_logic_find( uint64_t id )
{ return dsim_storage_find( ddl_sample->storage_logic, id ); }
inline static const uint64_t *sample_logic_id_data( uint32_t block ){ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_logic, block ); }
inline static const wire_ref *sample_logic_input_1_data( uint32_t block ){ return (const wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 0 ); }
inline static const wire_ref *sample_logic_input_2_data( uint32_t block ){ return (const wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 1 ); }
inline static const wire_ref *sample_logic_output_data( uint32_t block ){ return (const wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 2 ); }
inline static const logic_type *sample_logic_type_data( uint32_t block ){ return (const logic_type *)dsim_storage_data( ddl_sample->storage_logic, block, 3 ); }
inline static void sample_delay_insert( const uint64_t *ids, const wire_ref *input, const wire_ref *output, const float *delay, uint32_t count )
{ const void *_data[] = { input, output, delay }; dsim_storage_insert( ddl_sample->storage_delay, ids, _data, count ); }
inline static void sample_delay_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_delay, ids, count ); }
inline static dsim_storage_index sample_delay_find( uint64_t id )
{ return dsim_storage_find( ddl_sample->storage_delay, id ); }
inline static const uint64_t *sample_delay_id_data( uint32_t block ){ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_delay, block ); }
inline static const wire_ref *sample_delay_input_data( uint32_t block ){ return (const wire_ref *)dsim_storage_data( ddl_sample->storage_delay, block, 0 ); }
inline static const wire_ref *sample_delay_output_data( uint32_t block ){ return (const wire_ref *)dsim_storage_data( ddl_sample->storage_delay, block, 1 ); }
inline static const float *sample_delay_delay_data( uint32_t block ){ return (const float *)dsim_storage_data( ddl_sample->storage_delay, block, 2 ); }

void dsim_ddl_init_sample();
void dsim_ddl_done_sample();


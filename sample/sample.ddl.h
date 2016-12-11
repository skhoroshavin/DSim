
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

inline static uint8_t *sample_wires_state_data( uint32_t block ) { return (uint8_t *)dsim_storage_data( ddl_sample->storage_wires, block, 0 ); }
inline static wire_ref *sample_logic_input_1_data( uint32_t block ) { return (wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 0 ); }
inline static wire_ref *sample_logic_input_2_data( uint32_t block ) { return (wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 1 ); }
inline static wire_ref *sample_logic_output_data( uint32_t block ) { return (wire_ref *)dsim_storage_data( ddl_sample->storage_logic, block, 2 ); }
inline static logic_type *sample_logic_type_data( uint32_t block ) { return (logic_type *)dsim_storage_data( ddl_sample->storage_logic, block, 3 ); }
inline static wire_ref *sample_delay_input_data( uint32_t block ) { return (wire_ref *)dsim_storage_data( ddl_sample->storage_delay, block, 0 ); }
inline static wire_ref *sample_delay_output_data( uint32_t block ) { return (wire_ref *)dsim_storage_data( ddl_sample->storage_delay, block, 1 ); }
inline static float *sample_delay_delay_data( uint32_t block ) { return (float *)dsim_storage_data( ddl_sample->storage_delay, block, 2 ); }
void dsim_ddl_init_sample();
void dsim_ddl_done_sample();


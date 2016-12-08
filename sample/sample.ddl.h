
#pragma once

#include "table/table.h"

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

inline static uint8_t * wire_state_data( struct dsim_table * t, uint32_t block ) { return (uint8_t*)dsim_table_data( t, block, 0 ); }

inline static wire_ref * logic_input_1_data( struct dsim_table * t, uint32_t block ) { return (wire_ref*)dsim_table_data( t, block, 0 ); }
inline static wire_ref * logic_input_2_data( struct dsim_table * t, uint32_t block ) { return (wire_ref*)dsim_table_data( t, block, 1 ); }
inline static wire_ref * logic_output_data( struct dsim_table * t, uint32_t block ) { return (wire_ref*)dsim_table_data( t, block, 2 ); }
inline static logic_type * logic_type_data( struct dsim_table * t, uint32_t block ) { return (logic_type*)dsim_table_data( t, block, 3 ); }

inline static wire_ref * delay_input_data( struct dsim_table * t, uint32_t block ) { return (wire_ref*)dsim_table_data( t, block, 0 ); }
inline static wire_ref * delay_output_data( struct dsim_table * t, uint32_t block ) { return (wire_ref*)dsim_table_data( t, block, 1 ); }
inline static float * delay_delay_data( struct dsim_table * t, uint32_t block ) { return (float*)dsim_table_data( t, block, 2 ); }

void dsim_ddl_register_sample();


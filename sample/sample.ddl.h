
#pragma once

#include "storage/ddl_reader.h"
#include "storage/storage.h"

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

/////////////////////////////////////////////////////////////
// sample wires
/////////////////////////////////////////////////////////////

inline static void sample_wires_insert( const uint64_t *ids, const uint8_t *state, uint32_t count )
{ const void *_data[] = { state }; dsim_storage_insert( ddl_sample->storage_wires, ids, _data, count ); }
inline static void sample_wires_update( const uint64_t *ids, const uint8_t *state, uint32_t count )
{ const void *_data[] = { state }; dsim_storage_update( ddl_sample->storage_wires, ids, _data, count ); }
inline static void sample_wires_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_wires, ids, count ); }
inline static void sample_wires_select( const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{ dsim_storage_select_buf( ddl_sample->storage_wires, ids, addr, count ); }
inline static int sample_wires_can_modify( uint32_t arr )
{ return dsim_storage_can_modify( ddl_sample->storage_wires, 0, arr ); }

inline static const uint64_t *sample_wires_id_data()
{ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_wires, 0 ); }

inline static const uint8_t *sample_wires_state_begin_read() { return (const uint8_t *)dsim_storage_begin_read( ddl_sample->storage_wires, 0, 0 ); }
inline static uint8_t *sample_wires_state_begin_write( enum dsim_storage_write_mode mode ) { return (uint8_t *)dsim_storage_begin_write( ddl_sample->storage_wires, 0, 0, mode ); }

inline static void sample_wires_end_read( const void *data ) { dsim_storage_end_read( ddl_sample->storage_wires, data ); }
inline static void sample_wires_end_write( void *data ) { dsim_storage_end_write( ddl_sample->storage_wires, data ); }

/////////////////////////////////////////////////////////////
// sample logic
/////////////////////////////////////////////////////////////

inline static void sample_logic_insert( const uint64_t *ids, const wire_ref *input_1, const wire_ref *input_2, const wire_ref *output, const logic_type *type, uint32_t count )
{ const void *_data[] = { input_1, input_2, output, type }; dsim_storage_insert( ddl_sample->storage_logic, ids, _data, count ); }
inline static void sample_logic_update( const uint64_t *ids, const wire_ref *input_1, const wire_ref *input_2, const wire_ref *output, const logic_type *type, uint32_t count )
{ const void *_data[] = { input_1, input_2, output, type }; dsim_storage_update( ddl_sample->storage_logic, ids, _data, count ); }
inline static void sample_logic_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_logic, ids, count ); }
inline static void sample_logic_select( const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{ dsim_storage_select_buf( ddl_sample->storage_logic, ids, addr, count ); }
inline static int sample_logic_can_modify( uint32_t arr )
{ return dsim_storage_can_modify( ddl_sample->storage_logic, 0, arr ); }

inline static const uint64_t *sample_logic_id_data()
{ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_logic, 0 ); }

inline static const wire_ref *sample_logic_input_1_begin_read() { return (const wire_ref *)dsim_storage_begin_read( ddl_sample->storage_logic, 0, 0 ); }
inline static wire_ref *sample_logic_input_1_begin_write( enum dsim_storage_write_mode mode ) { return (wire_ref *)dsim_storage_begin_write( ddl_sample->storage_logic, 0, 0, mode ); }

inline static const wire_ref *sample_logic_input_2_begin_read() { return (const wire_ref *)dsim_storage_begin_read( ddl_sample->storage_logic, 0, 1 ); }
inline static wire_ref *sample_logic_input_2_begin_write( enum dsim_storage_write_mode mode ) { return (wire_ref *)dsim_storage_begin_write( ddl_sample->storage_logic, 0, 1, mode ); }

inline static const wire_ref *sample_logic_output_begin_read() { return (const wire_ref *)dsim_storage_begin_read( ddl_sample->storage_logic, 0, 2 ); }
inline static wire_ref *sample_logic_output_begin_write( enum dsim_storage_write_mode mode ) { return (wire_ref *)dsim_storage_begin_write( ddl_sample->storage_logic, 0, 2, mode ); }

inline static const logic_type *sample_logic_type_begin_read() { return (const logic_type *)dsim_storage_begin_read( ddl_sample->storage_logic, 0, 3 ); }
inline static logic_type *sample_logic_type_begin_write( enum dsim_storage_write_mode mode ) { return (logic_type *)dsim_storage_begin_write( ddl_sample->storage_logic, 0, 3, mode ); }

inline static void sample_logic_end_read( const void *data ) { dsim_storage_end_read( ddl_sample->storage_logic, data ); }
inline static void sample_logic_end_write( void *data ) { dsim_storage_end_write( ddl_sample->storage_logic, data ); }

/////////////////////////////////////////////////////////////
// sample delay
/////////////////////////////////////////////////////////////

inline static void sample_delay_insert( const uint64_t *ids, const wire_ref *input, const wire_ref *output, const float *delay, uint32_t count )
{ const void *_data[] = { input, output, delay }; dsim_storage_insert( ddl_sample->storage_delay, ids, _data, count ); }
inline static void sample_delay_update( const uint64_t *ids, const wire_ref *input, const wire_ref *output, const float *delay, uint32_t count )
{ const void *_data[] = { input, output, delay }; dsim_storage_update( ddl_sample->storage_delay, ids, _data, count ); }
inline static void sample_delay_remove( const uint64_t *ids, uint32_t count )
{ dsim_storage_remove( ddl_sample->storage_delay, ids, count ); }
inline static void sample_delay_select( const uint64_t *ids, dsim_storage_addr *addr, uint32_t count )
{ dsim_storage_select_buf( ddl_sample->storage_delay, ids, addr, count ); }
inline static int sample_delay_can_modify( uint32_t arr )
{ return dsim_storage_can_modify( ddl_sample->storage_delay, 0, arr ); }

inline static const uint64_t *sample_delay_id_data()
{ return (const uint64_t *)dsim_storage_id_data( ddl_sample->storage_delay, 0 ); }

inline static const wire_ref *sample_delay_input_begin_read() { return (const wire_ref *)dsim_storage_begin_read( ddl_sample->storage_delay, 0, 0 ); }
inline static wire_ref *sample_delay_input_begin_write( enum dsim_storage_write_mode mode ) { return (wire_ref *)dsim_storage_begin_write( ddl_sample->storage_delay, 0, 0, mode ); }

inline static const wire_ref *sample_delay_output_begin_read() { return (const wire_ref *)dsim_storage_begin_read( ddl_sample->storage_delay, 0, 1 ); }
inline static wire_ref *sample_delay_output_begin_write( enum dsim_storage_write_mode mode ) { return (wire_ref *)dsim_storage_begin_write( ddl_sample->storage_delay, 0, 1, mode ); }

inline static const float *sample_delay_delay_begin_read() { return (const float *)dsim_storage_begin_read( ddl_sample->storage_delay, 0, 2 ); }
inline static float *sample_delay_delay_begin_write( enum dsim_storage_write_mode mode ) { return (float *)dsim_storage_begin_write( ddl_sample->storage_delay, 0, 2, mode ); }

inline static void sample_delay_end_read( const void *data ) { dsim_storage_end_read( ddl_sample->storage_delay, data ); }
inline static void sample_delay_end_write( void *data ) { dsim_storage_end_write( ddl_sample->storage_delay, data ); }


void dsim_ddl_init_sample();
void dsim_ddl_done_sample();


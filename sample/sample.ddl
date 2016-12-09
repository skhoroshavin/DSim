{
    name: "sample",

    types: [
        { name: "uint8",  ctype: "uint8_t",  size: 1, data_type: numeric_type, data: { is_float: 0 } },
        { name: "uint16", ctype: "uint16_t", size: 2, data_type: numeric_type, data: { is_float: 0 } },
        { name: "uint32", ctype: "uint32_t", size: 4, data_type: numeric_type, data: { is_float: 0 } },
        { name: "uint64", ctype: "uint64_t", size: 8, data_type: numeric_type, data: { is_float: 0 } },

        { name: "float",  ctype: "float",  size: 4, data_type: numeric_type, data: { is_float: 1 } },
        { name: "double", ctype: "double", size: 8, data_type: numeric_type, data: { is_float: 1 } },

        { name: "vec2", data_type: struct_type,
            data: {
                fields: [
                    { name: "x", type: "float" },
                    { name: "y", type: "float" }
                ]
            }
        },

        { name: "vec3", data_type: struct_type,
            data: {
                fields: [
                    { name: "x", type: "float" },
                    { name: "y", type: "float" },
                    { name: "z", type: "float" }
                ]
            }
        },

        { name: "vec4", align: 16, data_type: struct_type,
            data: {
                fields: [
                    { name: "x", type: "float" },
                    { name: "y", type: "float" },
                    { name: "z", type: "float" },
                    { name: "w", type: "float" },
                ]
            }
        },

        { name: "wire_ref", data_type: reference_type,
            data: {
                target: "wires"
            }
        },

        { name: "logic_type", data_type: enum_type,
            data: {
                values: [ "and", "or", "nand", "nor", "xor" ]
            }
        }
    ],

    layouts: [
        {
            name: "wire",
            arrays: [
                { name: "state", type: "uint8" }
            ]
        },

        {
            name: "logic",
            arrays: [
                { name: "input_1", type: "wire_ref"   },
                { name: "input_2", type: "wire_ref"   },
                { name: "output",  type: "wire_ref"   },
                { name: "type",    type: "logic_type" }
            ]
        },

        {
            name: "delay",
            arrays: [
                { name: "input",   type: "wire_ref" },
                { name: "output",  type: "wire_ref" },
                { name: "delay",   type: "float"    }
            ]
        }
    ],

    storages: [
        { name: "wires", layout: "wire"  },
        { name: "logic", layout: "logic" },
        { name: "delay", layout: "delay" }
    ]
}

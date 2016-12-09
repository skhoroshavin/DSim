{
    name: "test",

    types: [
        { name: "uint8",  ctype: "uint8_t",  size: 1, data_type: numeric_type, data: { is_float: 0 } },
        { name: "float",  ctype: "float",  size: 4, data_type: numeric_type, data: { is_float: 1 } },

        { name: "vec4", align: 16, data_type: struct_type,
            data: {
                fields: [
                    { name: "x", type: "float" },
                    { name: "y", type: "float" },
                    { name: "z", type: "float" },
                    { name: "w", type: "float" },
                ]
            }
        }
    ],

    layouts: [
        {
            name: "test",
            arrays: [
                { name: "i", type: "uint8" },
                { name: "f", type: "float" },
                { name: "v", type: "vec4"  },
            ]
        }
    ]
}

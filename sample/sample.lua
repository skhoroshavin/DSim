
function process()
    local count = logic:count()
    for i = 1,count do
        local id = logic:get_id(i)
        local in1 = wires:state( logic:input_1(id) )
        local in2 = wires:state( logic:input_2(id) )
        wires:set_state( logic:output(id), in1 or in2 )
    end
end

wires:set_state( 1, 1 )

for i = 1,10 do
    process()
end

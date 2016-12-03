
function process()
    local count = logic:count()
    for i = 1,count do
        local id = logic:get_id(i)
        local in1 = wires:state( logic:input_1(id) )
        local in2 = wires:state( logic:input_2(id) )
        wires:set_state( logic:output(id), in1 or in2 )
    end
end

function dump()
    local count = wires:count()
    for i = 1,count do
        local id = wires:get_id(i)
        print( wires:state(id) )
    end
    print( "<<<<<<<<<<" )
end

wires:set_state( 1, 1 )

for i = 1,3 do
    dump()
    process()
end

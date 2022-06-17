
local bitarray = require("bitarray")

print("Start test bitarray")
-- local a = bitarray.new(100)
-- assert(bitarray.size(a) == 100, "bitarray size ~= 100")
-- for idx=1, 100 do 
--     bitarray.set(a, idx, idx % 2 == 0)
-- end
-- assert(bitarray.get(a, 10) == true)
-- assert(bitarray.get(a, 11) == false)

local oa = bitarray.new(66)
assert(oa:size() == 66, "bitarray size ~= 66")
for idx = 1, 66 do 
    oa:set(idx, idx % 2 == 0)
end
assert(oa:get(10) == true)
assert(oa:get(11) == false)
print("BitArray to string:", oa)

print("End test bitarray")
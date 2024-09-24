--[[
local model = Instance.new("Model")
local part1 = Instance.new("Part")
local part2 = Instance.new("Part")
local part3 = Instance.new("Part")

part1:SetParent(model)
part2:SetParent(part1)
part3:SetParent(part2)

print(#model:GetChildren())
print(#model:GetDescendants())

print("This is a normal message")
warn("This is a warning message")
error("This is an error messag

if Input.Keyboard:isKeyDown(Keycode.A) then
else 
    print("nope")
end
]]

print("Hello World")

local connection1 = Input.Keyboard.inputBegan:Connect(function(inputObject)
    print("input began")
end)

local connection2 = Input.Keyboard.inputEnded:Connect(function(inputObject)
    print("input ended")
end)

--connection1:Disconnect()
--connection2:Disconnect()

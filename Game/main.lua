local model = Instance.new("Model")
local part1 = Instance.new("Part")
local part2 = Instance.new("Part")
local part3 = Instance.new("Part")

part1:SetParent(model)
part2:SetParent(part1)
part3:SetParent(part2)

print(#model:GetChildren())
print(#model:GetDescendants())
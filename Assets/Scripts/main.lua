print("hello baby")
local render_window = RenderWindow()
local entity_mgr = EntityManager()

render_window:AddEntityManager(entity_mgr)
--render_window:GetEntityManager(entity_mgr)

local entity_id = entity_mgr:AddEntity("cube")
print("entity id ", entity_id)
local e_0 = entity_mgr:GetEntity(entity_id)

local scale = e_0:GetScale()
print("entity scale ", scale.x)
while true do
	render_window:Update()
end
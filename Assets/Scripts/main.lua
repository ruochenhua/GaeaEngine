local do_update = true
local render_window = RenderWindow()
local entity_mgr = EntityManager()
local cam_mgr = CameraManager()
local msg_mgr = MessageManager()
local res_mgr = ResourcesManager()

local load_result = res_mgr:LoadModel("Sting", "../Assets/Models/Sting.fbx")
print("load result", load_result)

render_window:AddModule("EntityManager", entity_mgr)
render_window:AddModule("CameraManager", cam_mgr)
render_window:AddModule("MessageManager", msg_mgr)

local cam = Camera()
cam:SetEye(5.0, 5.0, 5.0)
cam:SetAt(0,0,0)
cam:SetUp(0,1,0)

cam_mgr:AddCamera("MainCam", cam)

---[[
local entity_id = entity_mgr:AddEntity("Sting")

print("entity id ", entity_id)
local e_0 = entity_mgr:GetEntity(entity_id)

local scale = e_0:GetScale()
print("entity scale ", scale.x, scale)
e_0:SetScale(1,2,3)
--]]
local function HandleMsg(msg_table)
    for _, v in ipairs(msg_table) do
        --escape key
        if v == 27 then
            do_update = false
        end
    end
end

while do_update do
	render_window:Update()
    local msg_table = table.pack(msg_mgr:GetKeyDownQueue())
    HandleMsg(msg_table)
end


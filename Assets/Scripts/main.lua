package.path = package.path .. ";../Assets/Scripts/?.lua;"

--local CameraControl = require("CameraControl")
--local KeyBind = require("KeyBind")

local render_window = RenderWindow()
local cam_mgr = CameraManager()
local msg_mgr = MessageManager()

local do_update = true

render_window:AddModule("CameraManager", cam_mgr)
render_window:AddModule("MessageManager", msg_mgr)


local entity_mgr = render_window:GetEntityManager()

--camera
--创建entity
local cam_id = entity_mgr:AddEntity()
local cam_entity = {id = cam_id}
print("cam id", cam_entity.id)
local entity_ins = entity_mgr:GetEntity(cam_id)

if entity_ins then
    local trans_comp = TransformComponent()
    entity_ins:AddComponent("Transform", trans_comp)

    local cam_comp = CameraComponent()
    entity_ins:AddComponent("Camera", cam_comp)

    --添加component
    cam_entity.component = {"Transform", "Camera"}

    --添加label
    cam_entity.label = {"Transform", "Camera"}
end

--check each label, and according to them, put entity to systems
for _, v in pairs(cam_entity.label) do
    if v == "Camera" then
        cam_mgr:AddCamera("MainCamera", cam_entity.id)
    end
end


--[[


local cam = Camera()

cam_mgr:AddCamera("MainCam", cam)

local function HandleMsg(msg_table)
    for _, v in ipairs(msg_table) do
        --escape key
        local key = v.key
        if  KeyBind[key] == "ESC" then
            do_update = false
        end

        CameraControl.HandleCameraMsg(v, cam)
    end
end

--]]

local last_time = GetTime()
while do_update do
    local current_time = GetTime()
    repeat
        current_time = GetTime()
    until current_time - last_time > 16
    last_time = current_time
    --print("updating")
    ---[[
    render_window:Update()
    --local msg_table = table.pack(msg_mgr:GetInputMsgQueue())
    --HandleMsg(msg_table)
    --]]
end


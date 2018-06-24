package.path = package.path .. ";../Assets/Scripts/?.lua;"

local CameraControl = require("CameraControl")
local KeyBind = require("KeyBind")

local render_window = RenderWindow()
local entity_mgr = EntityManager()
local cam_mgr = CameraManager()
local msg_mgr = MessageManager()
local res_mgr = ResourcesManager()
local do_update = true

local load_result = res_mgr:LoadModel("Sting", "../Assets/Models/Sting.fbx")

render_window:AddModule("CameraManager", cam_mgr)
render_window:AddModule("EntityManager", entity_mgr)
render_window:AddModule("MessageManager", msg_mgr)

local cam = Camera()
cam:SetEye(1.0, 1.0, 1.0)
cam:SetAt(0,0,0)
cam:SetUp(0,1,0)

cam_mgr:AddCamera("MainCam", cam)

local entity_id = entity_mgr:AddEntity("Sting")

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

while do_update do
    render_window:Update()
    local msg_table = table.pack(msg_mgr:GetInputMsgQueue())
    HandleMsg(msg_table)
end


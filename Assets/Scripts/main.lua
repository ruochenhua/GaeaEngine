package.path = package.path .. ";../Assets/Scripts/?.lua;"

local CameraControl = require("CameraControl")
local KeyBind = require("KeyBind")

local render_window = RenderWindow()
local entity_mgr = EntityManager()
local cam_mgr = CameraManager()
local msg_mgr = MessageManager()
local res_mgr = ResourcesManager()
local terrain_mgr = TerrainManager()


local height_map = {}
local map_height = 50
local map_width = 50
local scale_height = 10
local scale_width = 10

local PerlinNoise = require("PerlinNoise")

print("perlin noise", PerlinNoise.Noise2D(1, 1))

for i = 1, map_height do
    for j = 1, map_width do
        table.insert(height_map, PerlinNoise.Noise2D(i, j))
    end
end

terrain_mgr:CreateTerrain(height_map, map_height, map_width, scale_height, scale_width)

local do_update = true

--local load_result = res_mgr:LoadModel("Sting", "../Assets/Models/Sting.fbx")
local result = res_mgr:LoadModel("sniper", "../Assets/Models/sniper/sniper_BIN.fbx")

render_window:AddModule("CameraManager", cam_mgr)
render_window:AddModule("EntityManager", entity_mgr)
render_window:AddModule("MessageManager", msg_mgr)
render_window:AddModule("TerrainManager", terrain_mgr)

local cam = Camera()

cam_mgr:AddCamera("MainCam", cam)

local entity_id = entity_mgr:AddEntity("sniper")
local entity = entity_mgr:GetEntity(entity_id)
entity:SetScale(0.01,0.01,0.01)

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

local last_time = GetTime()
while do_update do
    local current_time = GetTime()
    repeat
        current_time = GetTime()
    until current_time - last_time > 16
    last_time = current_time
    render_window:Update()
    local msg_table = table.pack(msg_mgr:GetInputMsgQueue())
    HandleMsg(msg_table)
end


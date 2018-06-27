local KeyBind = require("KeyBind")

local CameraControl = {}

local mouse_down = false
local last_x = 0
local last_y = 0

function CameraControl.HandleCameraMsg(input_msg, camera)

    local type = input_msg.type
    local key = input_msg.key

    if KeyBind[type] == "k_down" or KeyBind[type] == "k_up" then
        -- keyboard message
        local eye = camera:GetEye()
        local at = camera:GetAt()
        local dir = MathHelper.Vec3Sub(at, eye)

        local up = camera:GetUp()
        local right = MathHelper.Vec3Cross(dir, up)

        if KeyBind[key] == "w" then
            camera:Move(dir.x, dir.y, dir.z)
        elseif KeyBind[key] == "s" then
            camera:Move(-dir.x, -dir.y, -dir.z)
        elseif KeyBind[key] == "a" then
            camera:Move(right.x, right.y, right.z)
        elseif KeyBind[key] == "d" then
            camera:Move(-right.x, -right.y, -right.z)
        elseif KeyBind[key] == "q" then
            camera:Rotate(0.03, 0)
        elseif KeyBind[key] == "e" then
            camera:Rotate(-0.03, 0)
        elseif KeyBind[key] == "z" then
            camera:Rotate(0, 0.03)
        elseif KeyBind[key] == "c" then
            camera:Rotate(0, -0.03)
        end

    elseif KeyBind[type] == "m_motion" or KeyBind[type] == "m_down" or KeyBind[type] == "m_up" then
        -- mouse message
        if KeyBind[type] == "m_down" then
            mouse_down = true
            last_x = input_msg.x
            last_y = input_msg.y
        elseif KeyBind[type] == "m_up" then
            mouse_down = false
        end

        if KeyBind[type] == "m_motion" then
            if mouse_down then
                if KeyBind[key] == "m_left" then

                    local delta_x = input_msg.x - last_x
                    local delta_y = input_msg.y - last_y
                    --print("xx", delta_x, "yy", delta_y)
                    camera:Rotate(delta_x*0.02, delta_y*0.02)

                    last_x = input_msg.x
                    last_y = input_msg.y

                elseif KeyBind[key] == "m_right" then
                   --todo
                end
            end
        end

    end




end

return CameraControl
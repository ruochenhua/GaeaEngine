local PerlinNoise = {}

local persistence = 0.5
local Number_Of_Octaves = 4
---[[
--根据(x,y)获取一个初步噪声值
local function Noise(x, y)
    local n = x + y * 5
    math.randomseed(n)

    return math.random(-10, 10)
end

--光滑噪声
local function SmoothedNoise(x, y)
    local corners = ( Noise(x-1, y-1)+Noise(x+1, y-1)+Noise(x-1, y+1)+Noise(x+1, y+1) ) / 16
    local sides = ( Noise(x-1, y) +Noise(x+1, y) +Noise(x, y-1) +Noise(x, y+1) ) / 8
    local center = Noise(x, y) / 4

    return corners + sides + center
end

--余弦插值
local function Cosine_Interpolate(a, b, x)
    local ft = x * 3.1415927;
    local f = (1 - math.cos(ft)) * 0.5;

    return a*(1-f) + b*f;
end

--获取插值噪声
local function InterpolatedNoise(x, y)
    local integer_X = math.floor(x);
    local fractional_X = x - integer_X;

    local integer_Y = math.floor(y);
    local fractional_Y = y - integer_Y;


    local v1 = SmoothedNoise(integer_X, integer_Y);
    local v2 = SmoothedNoise(integer_X + 1, integer_Y);
    local v3 = SmoothedNoise(integer_X, integer_Y + 1);
    local v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);

    local i1 = Cosine_Interpolate(v1, v2, fractional_X);
    local i2 = Cosine_Interpolate(v3, v4, fractional_X);

    return Cosine_Interpolate(i1, i2, fractional_Y);
end
--]]
--最终调用：根据(x,y)获得其对应的PerlinNoise值
function PerlinNoise.Noise2D(x, y)
    local total = 0;

    local p = persistence;

    local n = Number_Of_Octaves;

    for i=0, n do
        local frequency = 2 ^ i;
        local amplitude = p ^ i;

        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
    end

    return total;
end

return PerlinNoise
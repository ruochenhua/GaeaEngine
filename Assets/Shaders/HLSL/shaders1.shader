cbuffer cbPerObject
{
    float4x4 WVP;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
    VS_OUTPUT output;
    output.Pos = mul(inPos, WVP);
    output.Color.xyz = inNormal;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
}
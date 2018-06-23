cbuffer cbPerObject
{
    float4x4 world;
    float4x4 view;
    float4x4 project;
};

cbuffer camPerObject
{
    float3 cam_pos;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
    VS_OUTPUT output;
    output.Pos = mul(inPos, world);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, project);

    float3 N = mul(inNormal, (float3x3)world);
    N = normalize(N);

    float3 P = output.Pos.xyz;
    float3 emissive = float3(0.0, 0.0, 0.0);
    float3 ambient = float3(0.3, 0.3, 0.3);
    
    float3 L = normalize(float3(-1.0, -1.0, 1.0));
    float diffuseLight = max(dot(N, L), 0);
    float3 diffuse = diffuseLight;

    float3 V = normalize(cam_pos);
    float3 H = normalize(L + V);
    float specularLight = pow(max(dot(N, H), 0), 5.0);

    if(diffuseLight <= 0)
    {
        specularLight = 0;
    }

    float3 specular = specularLight;

    output.Color.xyz = emissive + ambient + diffuse + specular;
    output.Color.w = 1.0;
    
    //output.Color.xyz = inNormal;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.Color;
}
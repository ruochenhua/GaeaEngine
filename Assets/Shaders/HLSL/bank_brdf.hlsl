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
    float4 WorldPos : TEXCOORD0;
    float4 WorldNormal : TEXCOORD1;
};

VS_OUTPUT VS(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
    VS_OUTPUT output;
    output.Pos = mul(inPos, world);
    output.WorldPos = output.Pos;

    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, project);
    
    output.WorldNormal = mul(inNormal, world);

    return output;
}



float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 light_color = float3(0.5, 0.3, 0.8);
    //Ka, Kd, Ks was set to (1, 1, 1) for now
    float ka = 1.0;
    float kd = 1.0;
    float ks = 1.0;
    float shininess = 2.0f;

    float3 P = input.Pos.xyz;
    float3 N = normalize(input.WorldNormal);

    //ambient light
    float3 ambient = ka * light_color;

    //diffuse light
    //light dir, for now set manually
    float3 L = normalize(float3(1, 1, 1));
    float ln = max(dot(L, N), 0);
    float3 diffuse = kd * light_color * ln;

    //specular light
    float3 V = normalize(cam_pos - P);
    float3 H = normalize(L + V);

    float3 specular = float3(0, 0, 0);
    bool back = (dot(V, N) > 0) && (dot(L, N) > 0);
    if(back)
    {
        //tangent
        float3 T = normalize(cross(N, V));
        float a = dot(L, T);
        float b = dot(V, T);
        float c = sqrt(1-pow(a, 2.0))*sqrt(1-pow(b,2.0)) - a*b;
        float brdf = ks*pow(c, shininess);

        specular = brdf * light_color * ln;
    }

    float4 color = float4(0, 0, 0, 1);
    color.xyz = ambient + diffuse + specular;
    return color;
    
}
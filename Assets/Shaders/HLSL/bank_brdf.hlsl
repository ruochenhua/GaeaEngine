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

struct VS_INPUT
{
    float4 Pos : POSITION;   //顶点坐标
    float3 Norm : NORMAL;    //法向量
    float2 Tex: TEXCOORD0;   //纹理坐标
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : TEXCOORD0;
    float4 WorldNormal : TEXCOORD1;

    float2 Tex : TEXCOORD3;
};

Texture2D shaderTexture;
SamplerState SampleType;

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul(input.Pos, world);
    output.WorldPos = output.Pos;

    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, project);
    
    output.WorldNormal = mul(input.Norm, world);
    output.Tex = input.Tex;

    return output;
}



float4 PS(VS_OUTPUT input) : SV_TARGET
{
   // return textureColor;

    float3 light_color = float3(0.5, 0.3, 0.8);
    //Ka, Kd, Ks was set to (1, 1, 1) for now
    float ka = 1.0;
    float kd = 1.0;
    float ks = 1.0;
    float shininess = 0.5f;

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
   // color.xyz = ambient + diffuse + specular;
   // return objTexture.Sample(ObjSamplerState, input.Tex);
   float2 tex_coord = input.Tex;
   tex_coord.y = 1.0 - tex_coord.y;
   color = shaderTexture.Sample(SampleType, tex_coord);
   color.xyz += ambient + specular;
   return color; 
}
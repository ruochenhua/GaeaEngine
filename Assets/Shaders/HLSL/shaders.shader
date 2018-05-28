struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
}

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = mul(position,World);
	output.position = mul(output.position,View);
	output.position = mul(output.position,Projection);
	
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}
cbuffer ObjectConstantBuffer : register(b0)
{
    float4x4 MVP;
};

struct VertexShaderInput
{
	float3 Position : POSITION;
	float3 Color	: COLOR;
};

struct VertexShaderOutput
{
	float4 Color	: COLOR;
	float4 Position : SV_POSITION;
};


VertexShaderOutput main( VertexShaderInput IN )
{
	VertexShaderOutput OUT;
	OUT.Color = float4(IN.Color, 1.0f);
    OUT.Position = mul(float4(IN.Position, 1.0f), MVP);

	return OUT;
}
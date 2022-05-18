struct VertexLayout
{
	float3 Position : POSITION;
	float3 Color : COLOR;
};

struct VertexShaderOutput
{
	float4 Color : COLOR;
	float4 Position : SV_POSITION;
};

VertexShaderOutput main(VertexLayout IN)
{
	VertexShaderOutput OUT;

	OUT.Position = float4(IN.Position, 1.0);
	OUT.Color = float4(IN.Color, 1.0f);

	return OUT;
}
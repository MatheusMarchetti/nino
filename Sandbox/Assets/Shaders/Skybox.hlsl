//Vertex Shader

cbuffer SceneConstantBuffer : register(b0)
{
    matrix ViewProjection;
}

cbuffer ModelConstantBuffer : register(b1)
{
    matrix Model;
};

struct VertexShaderInput
{
    float3 Position : Position;
};

struct VertexShaderOutput
{
    float3 TexCoord : Texture_Coord;
    float4 Position : SV_Position;
};

VertexShaderOutput VSmain(VertexShaderInput IN)
{
    VertexShaderOutput OUT;
    
    matrix MVP = mul(Model, ViewProjection);
    
    OUT.Position = mul(float4(IN.Position, 1.0f), MVP).xyww;
    OUT.TexCoord = IN.Position;

    return OUT;
}

//Pixel Shader

TextureCube skyMap : register(t0);

SamplerState samplerState : register(s0);

struct PixelShaderInput
{
    float3 TexCoord : Texture_Coord;
};

float4 PSmain(PixelShaderInput IN) : SV_Target
{
    return skyMap.Sample(samplerState, IN.TexCoord);
}
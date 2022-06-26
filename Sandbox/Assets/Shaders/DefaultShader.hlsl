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
    float2 TexCoord : Texture_Coord;
};

struct VertexShaderOutput
{
    float2 TexCoord : Texture_Coord;
    float4 Position : SV_Position;
};

VertexShaderOutput VSmain(VertexShaderInput IN)
{
    VertexShaderOutput OUT;
    
    matrix MVP = mul(Model, ViewProjection);
    
    OUT.Position = mul(float4(IN.Position, 1.0f), MVP);
    OUT.TexCoord = IN.TexCoord;

    return OUT;
}

//Pixel Shader

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetalnessTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);

SamplerState samplerState : register(s0);

struct PixelShaderInput
{
    float2 TexCoord : Texture_Coord;
};

float4 PSmain(PixelShaderInput IN) : SV_Target
{  
    return AlbedoTexture.Sample(samplerState, IN.TexCoord);
}
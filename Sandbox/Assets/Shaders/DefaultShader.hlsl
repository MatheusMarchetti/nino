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
    float4 Position : SV_Position;
};

VertexShaderOutput VSmain(VertexShaderInput IN)
{
    VertexShaderOutput OUT;
    
    matrix MVP = mul(Model, ViewProjection);
    
    OUT.Position = mul(float4(IN.Position, 1.0f), MVP);

    return OUT;
}

//Pixel Shader

struct PixelShaderInput
{

};

float4 PSmain(PixelShaderInput IN) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
//Vertex Shader

cbuffer SceneConstantBuffer : register(b0)
{
    matrix ViewProjection;
    float3 EyePosition;
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

Texture2D AlbedoTexture         : register(t1);
Texture2D NormalTexture         : register(t2);
Texture2D MetalnessTexture      : register(t3);
Texture2D RoughnessTexture      : register(t4);
Texture2D AOTexture             : register(t5);
TextureCube specularTexture     : register(t6);
TextureCube irradianceTexture   : register(t7);
Texture2D specularBRDF_LUT      : register(t8);

SamplerState defaultSampler : register(s0);
SamplerState spBRDF_Sampler : register(s1);

struct PixelShaderInput
{
    float2 TexCoord : Texture_Coord;
};

float4 PSmain(PixelShaderInput IN) : SV_Target
{
    float3 albedo = AlbedoTexture.Sample(defaultSampler, IN.TexCoord).rgb;
    float metalness = MetalnessTexture.Sample(defaultSampler, IN.TexCoord).r;
    float roughness = RoughnessTexture.Sample(defaultSampler, IN.TexCoord).r;
    
    float3 normal = normalize(2.0 * NormalTexture.Sample(defaultSampler, IN.TexCoord).rgb - 1.0);
    
    float3 irradiance = irradianceTexture.Sample(defaultSampler, normal).rgb;
    
    return float4(albedo, 1.0);
}
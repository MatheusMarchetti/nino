//Vertex Shader

struct VertexShaderInput
{

};

struct VertexShaderOutput
{

};

VertexShaderOutput VSmain(VertexShaderInput IN)
{
    VertexShaderOutput OUT;

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
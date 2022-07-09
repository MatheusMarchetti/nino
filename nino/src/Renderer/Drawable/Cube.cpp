#include "corepch.h"
#include "Cube.h"

#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
    Cube::Cube(const Material& material)
    {
        if (!IsStaticInitialized())
        {
            struct Vertex
            {
                DirectX::XMFLOAT3 Position;
                DirectX::XMFLOAT2 TextureCoords;
            };

            auto model = Cube::Make<Vertex>();

            AddStaticBind(CreateScope<VertexBuffer>(model.Vertices));
            AddStaticBind(CreateScope<IndexBuffer>(model.Indices));
            AddStaticBind(CreateScope<PixelShader>(material.GetShaderFile()));

            auto vertexShader = CreateScope<VertexShader>(material.GetShaderFile());

            BufferLayout layout = {
                {"Position", ShaderDataType::Float3_32b},
                {"Texture_Coord", ShaderDataType::Float2_32b}
            };

            AddStaticBind(CreateScope<InputLayout>(vertexShader->GetByteCode(), layout));
            AddStaticBind(std::move(vertexShader));
            AddStaticBind(CreateScope<Topology>(TopologyType::TriangleList));
            AddStaticBind(CreateScope<Sampler>());
        }
        else
        {
            SetIndexFromStatic();
        }

        m_ModelTransform = CreateRef<ModelConstantBuffer>();

        AddBind(m_ModelTransform);

        // Texture slot 0 reserved to cubemap

        AddBind(CreateScope<Texture>(material.GetAlbedoMap(), 1));
        AddBind(CreateScope<Texture>(material.GetNormalMap(), 2));
        AddBind(CreateScope<Texture>(material.GetMetallicMap(), 3));
        AddBind(CreateScope<Texture>(material.GetRoughnessMap(), 4));
//      AddBind(CreateScope<Texture>(material.GetAOMap(), 5));
    }
}

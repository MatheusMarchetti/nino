#include "corepch.h"
#include "Cube.h"

#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
    Cube::Cube()
    {
        if (!IsStaticInitialized())
        {
            struct Vertex
            {
                DirectX::XMFLOAT3 Position;
            };

            auto model = Cube::Make<Vertex>();

            AddStaticBind(CreateScope<VertexBuffer>(model.Vertices));
            AddStaticBind(CreateScope<IndexBuffer>(model.Indices));
            AddStaticBind(CreateScope<PixelShader>());

            auto vertexShader = CreateScope<VertexShader>();

            BufferLayout layout = {
                {"Position", ShaderDataType::Float3_32b}
            };

            AddStaticBind(CreateScope<InputLayout>(vertexShader->GetByteCode(), layout));
            AddStaticBind(std::move(vertexShader));
            AddStaticBind(CreateScope<Topology>(TopologyType::TriangleList));
        }
        else
        {
            SetIndexFromStatic();
        }

        m_ModelTransform = CreateRef<ModelConstantBuffer>();

        AddBind(m_ModelTransform);
    }
}

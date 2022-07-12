#include "corepch.h"
#include "Sphere.h"

#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
	Sphere::Sphere()
	{
		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 Position;
				DirectX::XMFLOAT2 TextureCoords;
			};

			auto model = Sphere::Make<Vertex>(12, 24);

			AddStaticBind(CreateScope<VertexBuffer>(model.Vertices));
			AddStaticBind(CreateScope<IndexBuffer>(model.Indices));
			AddStaticBind(CreateScope<PixelShader>());

			auto vertexShader = CreateScope<VertexShader>();

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
	}
}
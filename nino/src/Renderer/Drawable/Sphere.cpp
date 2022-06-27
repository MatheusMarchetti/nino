#include "corepch.h"
#include "Sphere.h"

#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
	Sphere::Sphere(const Material& material)
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

		if (!material.GetAlbedoMap().empty())
			AddBind(CreateScope<Texture>(material.GetAlbedoMap(), 0));

		if (!material.GetNormalMap().empty())
			AddBind(CreateScope<Texture>(material.GetNormalMap(), 1));

		if (!material.GetMetallicMap().empty())
			AddBind(CreateScope<Texture>(material.GetMetallicMap(), 2));

		if (!material.GetRoughnessMap().empty())
			AddBind(CreateScope<Texture>(material.GetRoughnessMap(), 3));

		if (!material.GetAOMap().empty())
			AddBind(CreateScope<Texture>(material.GetAOMap(), 4));
	}
}
#include "corepch.h"
#include "Skybox.h"

#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
	Skybox::Skybox(const std::string& skymap)
	{
		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 Position;
			};

			auto model = Skybox::Make<Vertex>(12, 24);

			AddStaticBind(CreateScope<VertexBuffer>(model.Vertices));
			AddStaticBind(CreateScope<IndexBuffer>(model.Indices));
			AddStaticBind(CreateScope<PixelShader>("Assets/Shaders/Skybox.hlsl"));

			auto vertexShader = CreateScope<VertexShader>("Assets/Shaders/Skybox.hlsl");

			BufferLayout layout = {
				{"Position", ShaderDataType::Float3_32b}
			};

			AddStaticBind(CreateScope<InputLayout>(vertexShader->GetByteCode(), layout));
			AddStaticBind(std::move(vertexShader));
			AddStaticBind(CreateScope<Topology>(TopologyType::TriangleList));
			AddStaticBind(CreateScope<Sampler>());
			AddStaticBind(CreateScope<RasterizerState>(CullingOptions::None));
			AddStaticBind(CreateScope<DepthStencilState>(DepthOptions::LessEqual));
			AddStaticBind(CreateScope<Texture>(skymap));
		}
		else
		{
			SetIndexFromStatic();
		}

		m_ModelTransform = CreateRef<ModelConstantBuffer>();

		AddBind(m_ModelTransform);
	}
}
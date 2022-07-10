#include "corepch.h"
#include "Skylight.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Bindable/ComputeShader.h"

namespace nino
{
	Skylight::Skylight(const std::string& filepath)
	{
		VertexBuffer vBuffer(m_Model.Vertices);
		IndexBuffer iBuffer(m_Model.Indices);
		PixelShader pShader("Assets/Shaders/Skybox.hlsl");
		VertexShader vShader("Assets/Shaders/Skybox.hlsl");

		BufferLayout layout = {
			{"Position", ShaderDataType::Float3_32b}
		};

		InputLayout iLayout(vShader.GetByteCode(), layout);
		Topology topology(TopologyType::TriangleList);
		Sampler sampler;
		RasterizerState rasterizer(CullingOptions::None);
		DepthStencilState dsState(DepthOptions::LessEqual);
		Texture texture(filepath);

		vBuffer.Bind();
		iBuffer.Bind();
		pShader.Bind();
		vShader.Bind();
		iLayout.Bind();
		topology.Bind();
		sampler.Bind();
		rasterizer.Bind();
		dsState.Bind();
		texture.Bind();
		m_Transform.Bind();
	}

	void Skylight::SetData(const ModelCBuf data)
	{
		auto context = GraphicsAPI::GetContext();

		m_Transform.SetData(data);

		GraphicsAPI::BindTargets();
		ThrowInfoMessage(context->DrawIndexed(*m_Model.Indices.data(), 0, 0));
	}
}


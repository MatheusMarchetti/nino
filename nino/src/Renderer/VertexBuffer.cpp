#include "corepch.h"
#include "VertexBuffer.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	VertexBuffer::VertexBuffer(GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI)
	{
	}

	void VertexBuffer::SetVertexBuffer(vec4f color, PrimitiveType primitive)
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto context = m_GraphicsAPI->GetContext();

		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresource;
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		switch (primitive)
		{
		case PrimitiveType::TRIANGLE:
		{
			Vertex m_Triangle[3] = {
			{{ 0.0f,  1.0f,  1.0f},  color},
			{{ 0.5f,  0.0f,  1.0f},  color},
			{{-0.5f,  0.0f,  1.0f},  color}
			};

			vertexDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(m_Triangle);

			vertexSubresource = { m_Triangle, 0, 0 };

			break;
		}

		case PrimitiveType::QUAD:
		{
			Vertex m_Quad[4] = {
			{{-0.5f,  0.5f, 1.0f}, color},
			{{ 0.5f,  0.5f, 1.0f}, color},
			{{ 0.5f, -0.5f, 1.0f}, color},
			{{-0.5f, -0.5f, 1.0f}, color},
			};

			vertexDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(m_Quad);

			vertexSubresource = { m_Quad, 0, 0 };

			break;
		}

		case PrimitiveType::CUBE:
		{
			Vertex m_Cube[8] = {
			{{-0.5f, -0.5f, 0.0f}, color},
			{{-0.5f,  0.5f, 0.0f}, color},
			{{ 0.5f,  0.5f, 0.0f}, color},
			{{ 0.5f, -0.5f, 0.0f}, color},
			{{-0.5f, -0.5f, 1.0f}, color},
			{{-0.5f,  0.5f, 1.0f}, color},
			{{ 0.5f,  0.5f, 1.0f}, color},
			{{ 0.5f, -0.5f, 1.0f}, color},
			};
			vertexDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(m_Cube);

			vertexSubresource = { m_Cube, 0, 0 };

			break;
		}
		}

		ThrowOnError(device->CreateBuffer(&vertexDesc, &vertexSubresource, &m_VertexBuffer));

		context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}

	void VertexBuffer::Release()
	{
		m_VertexBuffer = nullptr;

		m_GraphicsAPI = nullptr;
	}
}


#include "corepch.h"
#include "IndexBuffer.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	IndexBuffer::IndexBuffer(GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI)
	{

	}
	void IndexBuffer::SetIndexBuffer(PrimitiveType primitive)
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto context = m_GraphicsAPI->GetContext();

		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubresource;

		switch (primitive)
		{
		case PrimitiveType::TRIANGLE:
		{
			indexDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(m_TriangleIndices);

			indexSubresource = { m_TriangleIndices, 0, 0 };

			break;
		}

		case PrimitiveType::QUAD:
		{
			indexDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(m_QuadIndices);

			indexSubresource = { m_QuadIndices, 0, 0 };

			break;
		}
		}

		ThrowOnError(device->CreateBuffer(&indexDesc, &indexSubresource, &m_IndexBuffer));

		context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::Release()
	{
		m_IndexBuffer = nullptr;
		m_GraphicsAPI = nullptr;
	}
}


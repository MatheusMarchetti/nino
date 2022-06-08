#include "corepch.h"
#include "IndexBuffer.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices)
		: m_Count(indices.size())
	{
		auto device = GraphicsAPI::GetDevice();

		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.ByteWidth = sizeof(unsigned short) * indices.size();
		indexDesc.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA bufferdata = {};
		bufferdata.pSysMem = indices.data();

		ThrowOnError(device->CreateBuffer(&indexDesc, &bufferdata, &m_IndexBuffer));
	}

	void IndexBuffer::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	}

}


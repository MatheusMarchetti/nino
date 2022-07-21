#include "corepch.h"
#include "Buffer.h"

namespace nino
{
	IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indexData)
		: m_IndexCount(indexData.size())
	{
		auto device = GraphicsAPI::GetDevice();

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned short) * indexData.size();
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = indexData.data();

		ThrowOnError(device->CreateBuffer(&indexBufferDesc, &bufferData, &m_Buffer));
	}
}
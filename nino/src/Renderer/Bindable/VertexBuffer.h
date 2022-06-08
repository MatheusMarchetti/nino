#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	class VertexBuffer : public Bindable
	{
	public:
		template<typename T>
		VertexBuffer(const std::vector<T>& vertices)
			: m_Stride(sizeof(T))
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC vertexDesc = {};
			vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexDesc.ByteWidth = sizeof(T) * vertices.size();
			vertexDesc.StructureByteStride = sizeof(T);

			D3D11_SUBRESOURCE_DATA bufferdata = {};
			bufferdata.pSysMem = vertices.data();

			ThrowOnError(device->CreateBuffer(&vertexDesc, &bufferdata, &m_VertexBuffer));
		}

		virtual void Bind() override;

	private:
		UINT m_Stride;
		UINT m_Offset = 0;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};
}
#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Bindable/Bindable.h"


namespace nino
{
	template<typename T>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer()
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC cBufferDesc = {};
			cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cBufferDesc.ByteWidth = sizeof(T);

			ThrowOnError(device->CreateBuffer(&cBufferDesc, nullptr, &m_ConstantBuffer));
		}
		ConstantBuffer(const T& data)
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC cBufferDesc = {};
			cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cBufferDesc.ByteWidth = sizeof(T);

			D3D11_SUBRESOURCE_DATA bufferData = {};
			bufferData.pSysMem = &data;

			ThrowOnError(device->CreateBuffer(&cBufferDesc, &bufferData, &m_ConstantBuffer));
		}

		void Update(const T& data)
		{
			auto context = GraphicsAPI::GetContext();
			D3D11_MAPPED_SUBRESOURCE bufferData;

			context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &bufferData);
			memcpy(bufferData.pData, &data, sizeof(T));
			context->Unmap(m_ConstantBuffer.Get(), 0);
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	template<typename T>
	class VertexConstantBuffer : public ConstantBuffer<T>
	{
	public:
		virtual void Bind() override
		{
			Bind(0);
		}

		virtual void Bind(uint32_t slot)
		{
			auto context = GraphicsAPI::GetContext();

			context->VSSetConstantBuffers(slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<typename T>
	class PixelConstantBuffer : public ConstantBuffer<T>
	{
	public:
		virtual void Bind() override
		{
			Bind(0);
		}

		virtual void Bind(uint32_t slot)
		{
			auto context = GraphicsAPI::GetContext();

			context->PSSetConstantBuffers(slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	};
}
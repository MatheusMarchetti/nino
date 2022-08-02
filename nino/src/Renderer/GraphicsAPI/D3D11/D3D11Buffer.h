#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"

namespace nino
{
	class D3D11Buffer
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() { return m_Buffer; }

	protected:
		D3D11Buffer() = default;
		virtual ~D3D11Buffer() = default;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	};

	template<typename T>
	class D3D11VertexBuffer : public D3D11Buffer
	{
	public:
		D3D11VertexBuffer(const std::vector<T>& vertexData)
			: m_Stride(sizeof(T))
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(T) * vertexData.size();
			vertexBufferDesc.StructureByteStride = sizeof(T);

			D3D11_SUBRESOURCE_DATA bufferData = {};
			bufferData.pSysMem = vertexData.data();

			ThrowOnError(device->CreateBuffer(&vertexBufferDesc, &bufferData, &m_Buffer));
		}

		virtual ~D3D11VertexBuffer() = default;

	private:
		uint32_t m_Stride;
		uint32_t m_Offset;
	};

	class D3D11IndexBuffer : public D3D11Buffer
	{
	public:
		D3D11IndexBuffer(const std::vector<unsigned short>& indexData);
		virtual ~D3D11IndexBuffer() = default;

	private:
		uint32_t m_IndexCount;
	};

	template<typename T>
	class D3D11ConstantBuffer : public D3D11Buffer
	{
	public:
		D3D11ConstantBuffer()
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.ByteWidth = sizeof(T);

			ThrowOnError(device->CreateBuffer(&constantBufferDesc, nullptr, &m_Buffer));
		}

		D3D11ConstantBuffer(const T& cbufferData)
		{
			auto device = GraphicsAPI::GetDevice();

			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.ByteWidth = sizeof(T);

			D3D11_SUBRESOURCE_DATA bufferData = {};
			bufferData.pSysMem = &cbufferData;

			ThrowOnError(device->CreateBuffer(&constantBufferDesc, &bufferData, &m_Buffer));
		}

		void SetData(const T& cbufferData)
		{
			auto context = GraphicsAPI::GetContext();

			D3D11_MAPPED_SUBRESOURCE bufferData = {};

			context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &bufferData);
			memcpy(bufferData.pData, &cbufferData, sizeof(T));
			context->Unmap(m_Buffer.Get(), 0);
		}

		virtual ~D3D11ConstantBuffer() = default;
	};
}
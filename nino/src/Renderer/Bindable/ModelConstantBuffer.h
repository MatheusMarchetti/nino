#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/ConstantBuffer.h"

namespace nino
{
	struct ModelCBuf
	{
		DirectX::XMFLOAT4X4 ModelMatrix;
	};

	class ModelConstantBuffer : public Bindable
	{
	public:
		ModelConstantBuffer() = default;
		ModelConstantBuffer(const ModelCBuf& modelBuffer)
		{
			m_ModelBufferData = modelBuffer;
		}

		virtual void SetData(const ModelCBuf modelBuffer)
		{
			m_ModelBufferData = modelBuffer;
		}

		virtual void Bind() override
		{
			m_VertexConstantBuffer.Update(m_ModelBufferData);

			m_VertexConstantBuffer.Bind(1);
		}

	private:
		VertexConstantBuffer<ModelCBuf> m_VertexConstantBuffer;
		ModelCBuf m_ModelBufferData;
	};
}
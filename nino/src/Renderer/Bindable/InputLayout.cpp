#include "corepch.h"
#include "InputLayout.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	InputLayout::InputLayout(ID3DBlob* shaderByteCode, const BufferLayout& layout)
	{
		auto device = GraphicsAPI::GetDevice();
		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;

		for (const auto& entry : layout)
		{
			int index = (entry.Classification == ElementClassification::Vertex) ? 0 : 1;

			D3D11_INPUT_ELEMENT_DESC desc =
			{ entry.Name, 0, ShaderDataTypeSize(entry.DataType, entry.Normalized), index, D3D11_APPEND_ALIGNED_ELEMENT, ElementClassificationType(entry.Classification), entry.InstanceSize };
			
			elementDescs.push_back(desc);
		}

		ThrowOnError(device->CreateInputLayout(elementDescs.data(), elementDescs.size(), shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &m_InputLayout));
	}

	void InputLayout::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->IASetInputLayout(m_InputLayout.Get());
	}

	DXGI_FORMAT InputLayout::ShaderDataTypeSize(ShaderDataType type, bool normalized)
	{
		switch (type)
		{
		case ShaderDataType::Uint_8b:
		{
			if (normalized) return DXGI_FORMAT_R8_UNORM;
			return DXGI_FORMAT_R8_UINT;
		}
		case ShaderDataType::Uint_16b:
		{
			if (normalized) return DXGI_FORMAT_R16_UNORM;
			return DXGI_FORMAT_R16_UINT;
		}
		case ShaderDataType::Uint2_8b:
		{
			if (normalized) return DXGI_FORMAT_R8G8_UNORM;
			return DXGI_FORMAT_R8G8_UINT;
		}
		case ShaderDataType::Uint4_8b:
		{
			if (normalized) return DXGI_FORMAT_R8G8B8A8_UNORM;
			return DXGI_FORMAT_R8G8B8A8_UINT;
		}
		case ShaderDataType::Uint_32b:		return DXGI_FORMAT_R32_UINT;
		case ShaderDataType::Float_32b:		return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2_16b:	return DXGI_FORMAT_R16G16_FLOAT;
		case ShaderDataType::Float2_32b:	return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3_32b:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4_16b:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case ShaderDataType::Float4_32b:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	D3D11_INPUT_CLASSIFICATION InputLayout::ElementClassificationType(ElementClassification classification)
	{
		switch (classification)
		{
		case ElementClassification::Vertex:		return D3D11_INPUT_PER_VERTEX_DATA;
		case ElementClassification::Instance:	return D3D11_INPUT_PER_INSTANCE_DATA;
		}
	}
}


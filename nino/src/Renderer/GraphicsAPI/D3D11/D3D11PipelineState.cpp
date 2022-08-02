#include "corepch.h"
#include "D3D11PipelineState.h"

namespace nino
{
	SamplerState::SamplerState(Filter samplingFilter, TextureMode textureMode, ComparisonFunction comparisonFunc, uint32_t anisotropyLevel, const Color& borderColor)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = (D3D11_FILTER)samplingFilter;
		samplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)textureMode;
		samplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)textureMode;
		samplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)textureMode;
		samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)comparisonFunc;
		samplerDesc.MaxAnisotropy = anisotropyLevel;
		samplerDesc.BorderColor[0] = borderColor.x;
		samplerDesc.BorderColor[1] = borderColor.y;
		samplerDesc.BorderColor[2] = borderColor.z;
		samplerDesc.BorderColor[3] = borderColor.w;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateSamplerState(&samplerDesc, &m_SamplerState));
	}

	RasterizerState::RasterizerState(FillMode fillMode, CullMode cullMode)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = (D3D11_FILL_MODE)fillMode;
		rasterizerDesc.CullMode = (D3D11_CULL_MODE)cullMode;

		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
	}

	DepthStencilState::DepthStencilState(bool depthEnabled, bool depthWrite, ComparisonFunction comparisonFunc, bool stencilEnabled, uint8_t stencilRead, uint8_t stencilWrite)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = depthEnabled;
		depthStencilDesc.StencilEnable = stencilEnabled;
		depthStencilDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)depthWrite;
		depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)comparisonFunc;
		depthStencilDesc.StencilReadMask = stencilRead;
		depthStencilDesc.StencilWriteMask = stencilWrite;

		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));
	}

	PrimitiveTopology::PrimitiveTopology(TopologyType type)
	{
		auto context = D3D11Backend::GetContext();
		context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
	}

	InputLayout::InputLayout(ID3DBlob* VertexShaderBytecode, const BufferLayout& layout)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;

		for (const auto& entry : layout)
		{
			int index = (entry.Classification == ElementClassification::Vertex) ? 0 : 1;

			D3D11_INPUT_ELEMENT_DESC desc =
			{ entry.Name, 0, (DXGI_FORMAT)entry.DataType, index, D3D11_APPEND_ALIGNED_ELEMENT, (D3D11_INPUT_CLASSIFICATION)entry.Classification, entry.InstanceSize };

			elementDescs.push_back(desc);
		}

		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateInputLayout(elementDescs.data(), elementDescs.size(), VertexShaderBytecode->GetBufferPointer(), VertexShaderBytecode->GetBufferSize(), &m_InputLayout));
	}
}
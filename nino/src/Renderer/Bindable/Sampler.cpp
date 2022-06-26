#include "corepch.h"
#include "Sampler.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	Sampler::Sampler()
	{
		auto device = GraphicsAPI::GetDevice();

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		ThrowOnError(device->CreateSamplerState(&samplerDesc, &m_SamplerState));
	}

	void Sampler::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
	}
}


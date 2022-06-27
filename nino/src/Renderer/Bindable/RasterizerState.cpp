#include "corepch.h"
#include "RasterizerState.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	RasterizerState::RasterizerState(CullingOptions cullOptions)
	{
		auto device = GraphicsAPI::GetDevice();

		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(cullOptions);
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = TRUE;

		ThrowOnError(device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
	}

	void RasterizerState::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->RSSetState(m_RasterizerState.Get());
	}
}


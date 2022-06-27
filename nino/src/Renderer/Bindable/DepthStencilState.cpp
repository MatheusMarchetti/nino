#include "corepch.h"
#include "DepthStencilState.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	DepthStencilState::DepthStencilState(DepthOptions depthOptions)
	{
		auto device = GraphicsAPI::GetDevice();

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(depthOptions);

		ThrowOnError(device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));
	}

	void DepthStencilState::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);
	}
}

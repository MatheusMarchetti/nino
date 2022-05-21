#include "corepch.h"
#include "DepthStencil.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	DepthStencil::DepthStencil(uint32_t width, uint32_t height, GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI)
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto context = m_GraphicsAPI->GetContext();

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		device->CreateTexture2D(&depthStencilDesc, NULL, &m_DepthStencilBuffer);
		device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), NULL, &m_DepthStencilView);
	}

	void DepthStencil::Release()
	{
		m_DepthStencilBuffer = nullptr;
		m_DepthStencilView = nullptr;

		m_GraphicsAPI = nullptr;
	}
}


#include "corepch.h"
#include "RenderTargets.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	RenderTargets::RenderTargets(GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI)
	{
		CreateRenderTargetView();
	}

	void RenderTargets::SetRenderTargets()
	{
		auto context = m_GraphicsAPI->GetContext();

		context->OMSetRenderTargets(1, m_RenderTarget.GetAddressOf(), nullptr);
	}

	void RenderTargets::SetViewports(uint32_t width, uint32_t height)
	{
		auto context = m_GraphicsAPI->GetContext();

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = height;

		context->RSSetViewports(1, &viewport);
	}

	void RenderTargets::Draw(uint32_t numIndices)
	{
		auto context = m_GraphicsAPI->GetContext();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(numIndices, 0, 0);
	}

	void RenderTargets::Clear(float color[4], float depth)
	{
		auto context = m_GraphicsAPI->GetContext();

		context->ClearRenderTargetView(m_RenderTarget.Get(), color);
	}

	void RenderTargets::Present(bool vSync)
	{
		UINT syncInterval = vSync ? 1 : 0;

		m_GraphicsAPI->GetSwapChain()->Present(syncInterval, 0);
	}

	void RenderTargets::Release()
	{
		m_RenderTarget = nullptr;
		m_GraphicsAPI = nullptr;
	}

	void RenderTargets::CreateRenderTargetView()
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto swapChain = m_GraphicsAPI->GetSwapChain();

		ComPtr<ID3D11Texture2D> backbuffer;

		ThrowOnError(swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));
		ThrowOnError(device->CreateRenderTargetView(backbuffer.Get(), nullptr, &m_RenderTarget));
	}


}


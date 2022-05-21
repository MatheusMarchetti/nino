#include "corepch.h"
#include "RenderTargets.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	RenderTargets::RenderTargets(uint32_t width, uint32_t height, GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI), m_DepthStencil(width, height, graphicsAPI)
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto swapChain = m_GraphicsAPI->GetSwapChain();

		ComPtr<ID3D11Texture2D> backbuffer;

		ThrowOnError(swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));
		ThrowOnError(device->CreateRenderTargetView(backbuffer.Get(), nullptr, &m_RenderTarget));
	}

	void RenderTargets::SetRenderTargets()
	{
		auto context = m_GraphicsAPI->GetContext();
		auto& dsv = m_DepthStencil.GetDSV();

		context->OMSetRenderTargets(1, m_RenderTarget.GetAddressOf(), dsv.Get());
	}

	void RenderTargets::SetViewports(uint32_t width, uint32_t height)
	{
		auto context = m_GraphicsAPI->GetContext();

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

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
		auto& dsv = m_DepthStencil.GetDSV();

		context->ClearRenderTargetView(m_RenderTarget.Get(), color);
		context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
	}

	void RenderTargets::Present(bool vSync)
	{
		UINT syncInterval = vSync ? 1 : 0;

		m_GraphicsAPI->GetSwapChain()->Present(syncInterval, 0);
	}

	void RenderTargets::Release()
	{
		m_DepthStencil.Release();

		m_RenderTarget = nullptr;
		m_GraphicsAPI = nullptr;
	}
}


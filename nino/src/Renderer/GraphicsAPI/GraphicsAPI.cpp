#include "corepch.h"
#include "GraphicsAPI.h"

#include "Core/Window.h"

namespace nino
{
	Microsoft::WRL::ComPtr<ID3D11Device> GraphicsAPI::m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsAPI::m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> GraphicsAPI::m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GraphicsAPI::m_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GraphicsAPI::m_DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GraphicsAPI::m_DepthStencilState;
	BOOL GraphicsAPI::m_TearingSupport = FALSE;
	D3D11_VIEWPORT GraphicsAPI::m_Viewport;

	using namespace Microsoft::WRL;

	void GraphicsAPI::Init(Window* window)
	{
		ThrowOnError(CoInitialize(NULL));
		UINT deviceFlags = 0;

#ifdef CORE_DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		ThrowOnError(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &m_Device, nullptr, &m_DeviceContext));

		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ComPtr<IDXGIFactory7> dxgiFactory7;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;

		ThrowOnError(m_Device.As(&dxgiDevice4));
		ThrowOnError(dxgiDevice4->GetAdapter(&dxgiAdapter));
		ThrowOnError(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory7)));

		if (FAILED(dxgiFactory7->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_TearingSupport, sizeof(m_TearingSupport))))
		{
			m_TearingSupport = FALSE;
		}

		DXGI_SWAP_CHAIN_DESC1 desc1 = {};
		desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc1.BufferCount = s_BufferCount;
		desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc1.SampleDesc.Count = 1;
		desc1.Flags = m_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ThrowOnError(dxgiFactory7->CreateSwapChainForHwnd(dxgiDevice4.Get(), window->GetWindow(), &desc1, nullptr, NULL, &dxgiSwapChain1));
		ThrowOnError(dxgiFactory7->MakeWindowAssociation(window->GetWindow(), DXGI_MWA_NO_ALT_ENTER));
		ThrowOnError(dxgiSwapChain1.As(&m_SwapChain));

		ComPtr<ID3D11Texture2D> backBuffer;
		ComPtr<ID3D11Texture2D> depthStencil;

		D3D11_TEXTURE2D_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsvDesc.Width = window->GetWidth();
		dsvDesc.Height = window->GetHeight();
		dsvDesc.ArraySize = 1;
		dsvDesc.MipLevels = 1;
		dsvDesc.SampleDesc.Count = 1;
		dsvDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_DEPTH_STENCIL_DESC dsvStateDesc = {};
		dsvStateDesc.DepthEnable = TRUE;
		dsvStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsvStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		ThrowOnError(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		ThrowOnError(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView));
		ThrowOnError(m_Device->CreateTexture2D(&dsvDesc, nullptr, &depthStencil));
		ThrowOnError(m_Device->CreateDepthStencilState(&dsvStateDesc, &m_DepthStencilState));
		ThrowOnError(m_Device->CreateDepthStencilView(depthStencil.Get(), nullptr, &m_DepthStencilView));
	}

	void GraphicsAPI::SetViewport(uint32_t width, uint32_t height, uint32_t topX, uint32_t topY)
	{
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1;
		m_Viewport.TopLeftX = topX;
		m_Viewport.TopLeftY = topY;
	}

	void GraphicsAPI::Clear(float color[4], float depth)
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, NULL);
	}

	void GraphicsAPI::Present(bool vSync)
	{
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		UINT syncInterval = vSync ? 1 : 0;
		UINT flags = (m_TearingSupport == TRUE && vSync == false) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		m_SwapChain->Present(syncInterval, flags);
	}

	GraphicsAPI::~GraphicsAPI()
	{
		CoUninitialize();
	}
}
#include "corepch.h"
#include "GraphicsAPI.h"

#include "Core/Window.h"

#include "Debug/GraphicsInfo.h"

namespace nino
{
	BOOL GraphicsAPI::s_TearingSupport = TRUE;
	Microsoft::WRL::ComPtr<ID3D11Device> GraphicsAPI::s_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsAPI::s_DeviceContext;
	std::unordered_map<HWND, Microsoft::WRL::ComPtr<IDXGISwapChain4>> GraphicsAPI::s_RenderTargets;
	std::unordered_map<const char*, Framebuffer*> GraphicsAPI::s_Framebuffers;

	using namespace Microsoft::WRL;

	void GraphicsAPI::CreateDeviceAndContext()
	{
		ThrowOnError(CoInitialize(NULL));

		UINT deviceFlags = 0;

#ifdef CORE_DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		ThrowOnError(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &s_Device, nullptr, &s_DeviceContext));
	}

	void GraphicsAPI::CreateSwapChain(Window* window)
	{
		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ComPtr<IDXGIFactory5> dxgiFactory;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		ComPtr<IDXGISwapChain4> dxgiSwapChain4;

		ThrowOnError(s_Device.As(&dxgiDevice4));
		ThrowOnError(dxgiDevice4->GetAdapter(&dxgiAdapter));
		ThrowOnError(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		if (FAILED(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &s_TearingSupport, sizeof(s_TearingSupport))))
		{
			s_TearingSupport = FALSE;
		}

		DXGI_SWAP_CHAIN_DESC1 desc1 = {};
		desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc1.BufferCount = s_BufferCount;
		desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		desc1.SampleDesc.Count = 1;
		desc1.Flags = s_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ThrowOnError(dxgiFactory->CreateSwapChainForHwnd(dxgiDevice4.Get(), window->GetHandle(), &desc1, nullptr, NULL, &dxgiSwapChain1));
		ThrowOnError(dxgiFactory->MakeWindowAssociation(window->GetHandle(), DXGI_MWA_NO_ALT_ENTER));
		ThrowOnError(dxgiSwapChain1.As(&dxgiSwapChain4));

		s_RenderTargets[window->GetHandle()] = dxgiSwapChain4;
	}

	const Microsoft::WRL::ComPtr<IDXGISwapChain4> GraphicsAPI::GetSwapChain(const Window* window)
	{
		return s_RenderTargets[window->GetHandle()];
	}

	void GraphicsAPI::BindFramebuffers(const std::vector<Ref<Framebuffer>>& framebuffers)
	{
		for (size_t i = 0; i < framebuffers.size(); i++)
		{
			s_Framebuffers[framebuffers[i]->GetDescriptor().Name] = framebuffers[i].get();
		}

		std::vector<D3D11_VIEWPORT> viewports(s_Framebuffers.size());
		std::vector<ID3D11RenderTargetView*> rtvs(s_Framebuffers.size());

		for (auto& framebuffer : s_Framebuffers)
		{
			viewports.push_back(framebuffer.second->GetViewport());
			rtvs.push_back(framebuffer.second->GetRenderTarget());
		}

		s_DeviceContext->RSSetViewports(viewports.size(), viewports.data());
		s_DeviceContext->OMSetRenderTargets(s_Framebuffers.size(), rtvs.data(), s_Framebuffers.begin()->second->GetDepthStencil());
	}

	void GraphicsAPI::Present(bool vSync)
	{
		UINT syncInterval = vSync ? 1 : 0;
		UINT flags = (s_TearingSupport == TRUE && vSync == false) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		for (const auto& window : s_RenderTargets)
		{
			window.second->Present(syncInterval, flags);
		}
	}

	GraphicsAPI::~GraphicsAPI()
	{
		CoUninitialize();
	}
}
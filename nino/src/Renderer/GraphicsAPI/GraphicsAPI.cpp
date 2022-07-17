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
	std::unordered_map<std::string, Framebuffer*> GraphicsAPI::s_Framebuffers;

	static constexpr uint32_t s_BufferCount = 3;

	using namespace Microsoft::WRL;

	void GraphicsAPI::CreateDeviceAndContext()
	{
		ComPtr<IDXGIFactory5> dxgiFactory;

		ThrowOnError(CoInitialize(NULL));

		UINT deviceFlags = 0;

#ifdef CORE_DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		ThrowOnError(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &s_Device, nullptr, &s_DeviceContext));

		DXGI_ADAPTER_DESC adapterDesc = {};
		GetAdapter()->GetDesc(&adapterDesc);

		ThrowOnError(GetAdapter()->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		if (FAILED(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &s_TearingSupport, sizeof(s_TearingSupport))))
		{
			s_TearingSupport = FALSE;
		}

		std::filesystem::path adapterName = adapterDesc.Description;

		std::string vendor = "Unknown vendor";

		switch (adapterDesc.VendorId)
		{
		case 0x10de: vendor = "NVidia"; break;
		case 0x1002: vendor = "AMD"; break;
		}

		NINO_CORE_INFO("--------------------");
		NINO_CORE_INFO("GPU Information:");
		NINO_CORE_INFO("Model: {}", adapterName.string());
		NINO_CORE_INFO("Revision: {}", adapterDesc.Revision);
		NINO_CORE_INFO("Vendor: {}", vendor);
		NINO_CORE_INFO("VRAM: {} MB", adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		NINO_CORE_INFO("Shared system memory: {} MB", adapterDesc.SharedSystemMemory / 1024 / 1024);
		NINO_CORE_INFO("System memory: {} MB", adapterDesc.DedicatedSystemMemory / 1024 / 1024);
		NINO_CORE_INFO("--------------------");
		NINO_CORE_INFO("Graphics Information:");
		NINO_CORE_INFO("Screen tearing support? {}", (s_TearingSupport ? "True" : "False"));
		NINO_CORE_INFO("--------------------");
	}

	void GraphicsAPI::CreateSwapChain(Window* window)
	{
		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIFactory5> dxgiFactory;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		ComPtr<IDXGISwapChain4> dxgiSwapChain4;

		ThrowOnError(s_Device.As(&dxgiDevice4));

		ThrowOnError(GetAdapter()->GetParent(IID_PPV_ARGS(&dxgiFactory)));

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

	Microsoft::WRL::ComPtr<IDXGIAdapter> GraphicsAPI::GetAdapter()
	{
		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIAdapter> dxgiAdapter;

		ThrowOnError(s_Device.As(&dxgiDevice4));
		ThrowOnError(dxgiDevice4->GetAdapter(&dxgiAdapter));

		return dxgiAdapter;
	}

	const Microsoft::WRL::ComPtr<IDXGISwapChain4> GraphicsAPI::GetSwapChain(const Window* window)
	{
		return s_RenderTargets[window->GetHandle()];
	}

	void GraphicsAPI::BindFramebuffers(std::initializer_list<Ref<Framebuffer>> framebuffers)
	{
		std::vector<D3D11_VIEWPORT> viewports;
		std::vector<ID3D11RenderTargetView*> rtvs;
		viewports.reserve(framebuffers.size());
		rtvs.reserve(framebuffers.size());
		std::string mainFramebuffer = (*framebuffers.begin())->GetDescriptor().Name;

		for (auto& framebuffer : framebuffers)
		{
			s_Framebuffers[framebuffer->GetDescriptor().Name] = framebuffer.get();
			viewports.push_back(framebuffer->GetViewport());
			rtvs.push_back(framebuffer->GetRenderTarget());
		}

		s_DeviceContext->RSSetViewports(viewports.size(), viewports.data());
		s_DeviceContext->OMSetRenderTargets(framebuffers.size(), rtvs.data(), s_Framebuffers[mainFramebuffer]->GetDepthStencil());
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
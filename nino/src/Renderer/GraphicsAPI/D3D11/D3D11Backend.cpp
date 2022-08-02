#include "corepch.h"
#include "D3D11Backend.h"

#include "Core/Window.h"

#include "Debug/GraphicsInfo.h"

namespace nino
{
	using namespace Microsoft::WRL;

	D3D11Backend::D3D11Backend()
	{
		ComPtr<IDXGIFactory5> dxgiFactory;

		UINT deviceFlags = 0;

#ifdef CORE_DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		ThrowOnError(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &m_Device, nullptr, &m_DeviceContext));

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
		NINO_CORE_INFO("Current API: Direct3D 11.4");
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

	void D3D11Backend::CreateSwapChain(Window* window)
	{
		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIFactory5> dxgiFactory;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		ComPtr<IDXGISwapChain4> dxgiSwapChain4;

		ThrowOnError(m_Device.As(&dxgiDevice4));

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

	void D3D11Backend::Present(bool vSync)
	{
		UINT syncInterval = vSync ? 1 : 0;
		UINT flags = (s_TearingSupport == TRUE && vSync == false) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		for (const auto& window : s_RenderTargets)
		{
			window.second->Present(syncInterval, flags);
		}
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> D3D11Backend::GetAdapter()
	{
		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIAdapter> dxgiAdapter;

		ThrowOnError(m_Device.As(&dxgiDevice4));
		ThrowOnError(dxgiDevice4->GetAdapter(&dxgiAdapter));

		return dxgiAdapter;
	}
}

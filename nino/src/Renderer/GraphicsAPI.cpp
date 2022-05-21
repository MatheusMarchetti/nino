#include "corepch.h"
#include "GraphicsAPI.h"

#include "Core/Window.h"

namespace nino
{
	using namespace Microsoft::WRL;

	GraphicsAPI::GraphicsAPI(Window* window)
	{
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		UINT deviceFlags = 0;

#ifdef CORE_DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		ThrowOnError(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, &deviceContext));

		ThrowOnError(device.As(&m_Device));
		ThrowOnError(deviceContext.As(&m_DeviceContext));

		ComPtr<IDXGIDevice4> dxgiDevice4;
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ComPtr<IDXGIFactory7> dxgiFactory7;
		ComPtr<IDXGISwapChain1> dxgiSwapChain1;

		ThrowOnError(m_Device.As(&dxgiDevice4));
		ThrowOnError(dxgiDevice4->GetAdapter(&dxgiAdapter));
		ThrowOnError(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory7)));

		DXGI_SWAP_CHAIN_DESC1 desc1 = {};
		desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc1.BufferCount = s_BufferCount;
		desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc1.SampleDesc.Count = 1;

		ThrowOnError(dxgiFactory7->CreateSwapChainForHwnd(dxgiDevice4.Get(), window->GetWindow(), &desc1, nullptr, NULL, &dxgiSwapChain1));

		ThrowOnError(dxgiSwapChain1.As(&m_SwapChain));
	}

	void GraphicsAPI::Release()
	{
		m_SwapChain = nullptr;
		m_DeviceContext = nullptr;
		m_Device = nullptr;
	}
}
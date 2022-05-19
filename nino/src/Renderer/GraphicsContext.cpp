#include "corepch.h"
#include "GraphicsContext.h"

#include "Renderer/GraphicsAPI.h"
#include "Renderer/CommandManager.h"

namespace nino
{
	using namespace Microsoft::WRL;

	GraphicsContext::GraphicsContext(const HWND& window, const uint32_t width, const uint32_t height, CommandManager* commandManager)
		: m_TearingSupport(false), m_CommandManager(commandManager)
	{
		m_TearingSupport = CheckTearingSupport();
		m_SwapChain = CreateSwapChain(window, width, height);
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain4> GraphicsContext::CreateSwapChain(const HWND& window, const uint32_t width, const uint32_t height)
	{
		ComPtr<IDXGISwapChain1> swapChain1;
		ComPtr<IDXGISwapChain4> swapChain4;
		ComPtr<IDXGIFactory4> dxgiFactory4;
		UINT createFactoryFlags = 0;

#ifdef CORE_DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowOnError(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.BufferCount = s_BufferCount;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Flags = m_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0 | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ThrowOnError(dxgiFactory4->CreateSwapChainForHwnd(m_CommandManager->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT).Get(), window, &swapChainDesc, nullptr, nullptr, &swapChain1));

		ThrowOnError(dxgiFactory4->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER));

		ThrowOnError(swapChain1.As(&swapChain4));

		return swapChain4;
	}

	bool GraphicsContext::CheckTearingSupport()
	{
		BOOL allowTearing = FALSE;

		ComPtr<IDXGIFactory4> factory4;

		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
		{
			ComPtr<IDXGIFactory5> factory5;

			if (SUCCEEDED(factory4.As(&factory5)))
			{
				factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
			}
		}

		return allowTearing ? true : false;
	}

	GraphicsContext::~GraphicsContext()
	{
		m_SwapChain->Release();
	}
}
#include "corepch.h"
#include "RendererAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	RendererAPI::~RendererAPI()
	{
		ThrowOnError(CloseHandle(m_FenceEvent));
	}

	void RendererAPI::Create(HWND& window, const uint32_t& width, const uint32_t& height)
	{
		m_Window = window;
		m_Width = width;
		m_Height = height;

		EnableDebugLayer();

		m_SelectedAdapter = m_DefaultAdapter ? QueryAdapters() : SelectedAdapter();

		DXGI_ADAPTER_DESC1 adapterDesc;
		m_SelectedAdapter->GetDesc1(&adapterDesc);

		NINO_CORE_INFO(L"Selected adapter: {}", adapterDesc.Description);

		m_Device = CreateDevice();

		m_Tearing = CheckTearingSupport();

		D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
		commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDesc.NodeMask = 0;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		ThrowOnError(m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_DirectCommandQueue)));

		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
		ThrowOnError(m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CopyCommandQueue)));

		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		ThrowOnError(m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_ComputeCommandQueue)));

		m_SwapChain = CreateSwapChain();
		m_CurrentBackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc;
		descriptorHeapDesc.NodeMask = 0;
		descriptorHeapDesc.NumDescriptors = s_BufferCount;
		descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		for (int i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; i++)
		{
			descriptorHeapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i);
			ThrowOnError(m_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_DescriptorHeaps[i])));

			m_DescriptorHeapsSizes[i] = m_Device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i));
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_DescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV]->GetCPUDescriptorHandleForHeapStart());

		for (int i = 0; i < s_BufferCount; i++)
		{
			ThrowOnError(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_Buffers[i])));

			m_Device->CreateRenderTargetView(m_Buffers[i].Get(), nullptr, rtvHandle);

			rtvHandle.Offset(m_DescriptorHeapsSizes[D3D12_DESCRIPTOR_HEAP_TYPE_RTV]);

			ThrowOnError(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocators[i])));
		}

		ThrowOnError(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocators[m_CurrentBackBufferIndex].Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));

		ThrowOnError(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		m_FenceEvent = ThrowOnError(CreateEvent(NULL, FALSE, FALSE, NULL));

		NINO_CORE_INFO(L"Direct3D12 initialized.");
	}

	void RendererAPI::Resize(const uint32_t& width, const uint32_t& height)
	{
		m_Width = width;
		m_Height = height;
	}

	void RendererAPI::EnableDebugLayer()
	{
#ifdef CORE_DEBUG
		ComPtr<ID3D12Debug1> debugInterface;

		ThrowOnError(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

		debugInterface->EnableDebugLayer();
#endif
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter4> RendererAPI::QueryAdapters()
	{
		ComPtr<IDXGIFactory4> dxgiFactory4;
		UINT createFactoryFlags = 0;

#ifdef CORE_DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowOnError(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

		ComPtr<IDXGIAdapter1> dxgiAdapter1;
		ComPtr<IDXGIAdapter4> dxgiAdapter4;

		for (UINT i = 0; dxgiFactory4->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC1 adapterDesc;

			dxgiAdapter1->GetDesc1(&adapterDesc);

			if ((adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device2), nullptr)))
			{
				ThrowOnError(dxgiAdapter1.As(&dxgiAdapter4));
				m_AvailableAdapters[adapterDesc.Description] = dxgiAdapter4;
			}
		}

		if (m_AvailableAdapters.empty())
		{
			ThrowOnError(dxgiFactory4->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
			ThrowOnError(dxgiAdapter1.As(&dxgiAdapter4));

			return dxgiAdapter4;
		}

		SIZE_T maxVRAM = 0;

		for (auto& adapter : m_AvailableAdapters)
		{
			DXGI_ADAPTER_DESC1 adapterDesc;
			adapter.second->GetDesc1(&adapterDesc);

			if (adapterDesc.DedicatedVideoMemory > maxVRAM)
			{
				 dxgiAdapter4 = adapter.second;
				maxVRAM = adapterDesc.DedicatedVideoMemory;
			}
		}

		return dxgiAdapter4;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter4> RendererAPI::SelectedAdapter()
	{
		ComPtr<IDXGIFactory4> dxgiFactory4;
		UINT createFactoryFlags = 0;

#ifdef CORE_DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowOnError(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

		ComPtr<IDXGIAdapter1> dxgiAdapter1;
		ComPtr<IDXGIAdapter4> dxgiAdapter4;

		for (UINT i = 0; dxgiFactory4->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC1 adapterDesc;

			dxgiAdapter1->GetDesc1(&adapterDesc);

			if ((adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device2), nullptr)))
			{
				ThrowOnError(dxgiAdapter1.As(&dxgiAdapter4));
				m_AvailableAdapters[adapterDesc.Description] = dxgiAdapter4;
			}
		}

		dxgiAdapter4 = m_AvailableAdapters[m_SelectedAdapterName];

		return dxgiAdapter4;
	}

	Microsoft::WRL::ComPtr<ID3D12Device2> RendererAPI::CreateDevice()
	{
		ComPtr<ID3D12Device2> device2;

		std::vector<D3D_FEATURE_LEVEL> featureLevels =
		{
			D3D_FEATURE_LEVEL_12_2,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		for (auto& featureLevel : featureLevels)
		{
			if (SUCCEEDED(D3D12CreateDevice(m_SelectedAdapter.Get(), featureLevel, IID_PPV_ARGS(&device2))))
			{
#ifdef CORE_DEBUG
				ComPtr<ID3D12InfoQueue> infoQueue;

				if (SUCCEEDED(device2.As(&infoQueue)))
				{
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
					infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

					D3D12_MESSAGE_SEVERITY denySeverities[] = { D3D12_MESSAGE_SEVERITY_INFO };
					D3D12_MESSAGE_ID denyIds[] =
					{
						D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
						D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
						D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
					};

					D3D12_INFO_QUEUE_FILTER newFilter = {};
					newFilter.DenyList.NumSeverities = _countof(denySeverities);
					newFilter.DenyList.pSeverityList = denySeverities;
					newFilter.DenyList.NumIDs = _countof(denyIds);
					newFilter.DenyList.pIDList = denyIds;

					ThrowOnError(infoQueue->PushStorageFilter(&newFilter));
				}
#endif
				return device2;
			}
		}
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain4> RendererAPI::CreateSwapChain()
	{
		ComPtr<IDXGISwapChain1> swapChain1;
		ComPtr<IDXGISwapChain4> swapChain4;
		ComPtr<IDXGIFactory4> factory4;
		UINT createFactoryFlags = 0;

#ifdef CORE_DEBUG
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowOnError(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory4)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.BufferCount = s_BufferCount;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Width = m_Width;
		swapChainDesc.Height = m_Height;
		swapChainDesc.Flags = m_Tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ThrowOnError(factory4->CreateSwapChainForHwnd(m_DirectCommandQueue.Get(), m_Window, &swapChainDesc, nullptr, nullptr, &swapChain1));

		ThrowOnError(factory4->MakeWindowAssociation(m_Window, DXGI_MWA_NO_ALT_ENTER));

		ThrowOnError(swapChain1.As(&swapChain4));

		return swapChain4;
	}

	void RendererAPI::ToggleVSync(bool vsync)
	{
		m_VSync = vsync;
	}

	bool RendererAPI::CheckTearingSupport()
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
}


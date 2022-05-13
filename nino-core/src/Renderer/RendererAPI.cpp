#include "corepch.h"
#include "RendererAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;
	using namespace DirectX;

	RendererAPI::~RendererAPI()
	{
		WaitForPreviousFrame();
		ReportAPIObjects();
	}

	void RendererAPI::Create(HWND& window, const uint32_t& width, const uint32_t& height)
	{
		m_Window = window;
		m_Width = width;
		m_Height = height;

		EnableDebugLayer();

		m_SelectedAdapter = m_DefaultAdapter ? QueryAdapters() : SelectedAdapter();

		DXGI_ADAPTER_DESC1 adapterDesc = {};
		m_SelectedAdapter->GetDesc1(&adapterDesc);

		NINO_CORE_INFO(L"Selected adapter: {}", adapterDesc.Description);

		m_Device = CreateDevice();

		m_Tearing = CheckTearingSupport();

		m_DirectCommandQueue = CreateCommandQueue();

		m_SwapChain = CreateSwapChain();

		CreateRenderTargets();

		for (int i = 0; i < s_BufferCount; i++)
		{
			m_CommandList[i] = CreateCommandList();
		}

		CreateFences();
	}

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> RendererAPI::CreateCommandQueue()
	{
		ComPtr<ID3D12CommandQueue> commandQueue;
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDesc.NodeMask = 0;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		ThrowOnError(m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));

		return commandQueue;
	}

	void RendererAPI::EnableDebugLayer()
	{
#ifdef CORE_DEBUG
		ComPtr<ID3D12Debug1> debugInterface;

		ThrowOnError(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

		debugInterface->EnableDebugLayer();
#endif
	}

	void RendererAPI::ReportAPIObjects()
	{
		ComPtr<IDXGIDebug1> dxgiDebug1;

		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug1))))
		{
			dxgiDebug1->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}
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
			DXGI_ADAPTER_DESC1 adapterDesc = {};

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
			DXGI_ADAPTER_DESC1 adapterDesc = {};
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
			DXGI_ADAPTER_DESC1 adapterDesc = {};

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
				break;
			}

		}

		return device2;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain4> RendererAPI::CreateSwapChain()
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
		swapChainDesc.Width = m_Width;
		swapChainDesc.Height = m_Height;
		swapChainDesc.Flags = m_Tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0 | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ThrowOnError(dxgiFactory4->CreateSwapChainForHwnd(m_DirectCommandQueue.Get(), m_Window, &swapChainDesc, nullptr, nullptr, &swapChain1));

		ThrowOnError(dxgiFactory4->MakeWindowAssociation(m_Window, DXGI_MWA_NO_ALT_ENTER));

		ThrowOnError(swapChain1.As(&swapChain4));

		m_CurrentBackBufferIndex = swapChain4->GetCurrentBackBufferIndex();

		return swapChain4;
	}

	void RendererAPI::CreateRenderTargets()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
		rtvDesc.NumDescriptors = s_BufferCount;
		rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDesc.NodeMask = 0;
		rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowOnError(m_Device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap)));
		m_RTVOffsetSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (unsigned int i = 0; i < s_BufferCount; i++)
		{
			ThrowOnError(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_Buffers[i])));
			m_Device->CreateRenderTargetView(m_Buffers[i].Get(), nullptr, rtvHandle);

			rtvHandle.Offset(m_RTVOffsetSize);
		}
	}

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RendererAPI::CreateCommandList()
	{
		ComPtr<ID3D12GraphicsCommandList> commandList;
		for (int i = 0; i < s_BufferCount; i++)
		{
			ThrowOnError(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocators[i])));

			m_CommandAllocators[i]->SetName(L"Direct Allocator " + i);
		}

		ThrowOnError(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocators[m_CurrentBackBufferIndex].Get(), nullptr, IID_PPV_ARGS(&commandList)));

		ThrowOnError(commandList->Close());

		return commandList;
	}

	void RendererAPI::CreateFences()
	{
		for (int i = 0; i < s_BufferCount; i++)
		{
			ThrowOnError(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fences[i])));

			m_FenceValues[i] = 0;
		}

		m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}

	void RendererAPI::Render()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr += m_CurrentBackBufferIndex * m_RTVOffsetSize;

		m_CommandList[m_CurrentBackBufferIndex]->SetGraphicsRootSignature(m_RootSignature.Get());
		m_CommandList[m_CurrentBackBufferIndex]->SetPipelineState(m_PSO.Get());

		m_CommandList[m_CurrentBackBufferIndex]->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		m_CommandList[m_CurrentBackBufferIndex]->RSSetViewports(1, &m_Viewport);
		m_CommandList[m_CurrentBackBufferIndex]->RSSetScissorRects(1, &m_ScissorRect);
		m_CommandList[m_CurrentBackBufferIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList[m_CurrentBackBufferIndex]->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		m_CommandList[m_CurrentBackBufferIndex]->IASetIndexBuffer(&m_IndexBufferView);

		m_CommandList[m_CurrentBackBufferIndex]->DrawIndexedInstanced(3, 1, 0, 0, 0);


		CD3DX12_RESOURCE_BARRIER renderToPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_Buffers[m_CurrentBackBufferIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		m_CommandList[m_CurrentBackBufferIndex]->ResourceBarrier(1, &renderToPresent);
		m_CommandList[m_CurrentBackBufferIndex]->Close();

		ID3D12CommandList* const commandLists[] = { m_CommandList[m_CurrentBackBufferIndex].Get() };

		m_DirectCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

		ThrowOnError(m_DirectCommandQueue->Signal(m_Fences[m_CurrentBackBufferIndex].Get(), m_FenceValues[m_CurrentBackBufferIndex]));

		m_SwapChain->Present(m_VSync ? 1 : 0, 0);

		WaitForPreviousFrame();

		m_CurrentBackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
	}

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr += m_CurrentBackBufferIndex * m_RTVOffsetSize;

		ThrowOnError(m_CommandAllocators[m_CurrentBackBufferIndex]->Reset());
		ThrowOnError(m_CommandList[m_CurrentBackBufferIndex]->Reset(m_CommandAllocators[m_CurrentBackBufferIndex].Get(), nullptr));

		CD3DX12_RESOURCE_BARRIER presentToRender = CD3DX12_RESOURCE_BARRIER::Transition(m_Buffers[m_CurrentBackBufferIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_CommandList[m_CurrentBackBufferIndex]->ResourceBarrier(1, &presentToRender);

		FLOAT color[4] = { r, g, b, a };

		m_CommandList[m_CurrentBackBufferIndex]->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	}

	void RendererAPI::CreateRootSignature()
	{
		D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
		samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc[0].MipLODBias = 0;
		samplerDesc[0].MaxAnisotropy = 0;
		samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc[0].MinLOD = 0.0f;
		samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc[0].ShaderRegister = 0;
		samplerDesc[0].RegisterSpace = 0;
		samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Init(0, nullptr, 0, samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signatureBlob;
		ComPtr<ID3DBlob> errorBlob;

		ThrowOnError(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob));

		ThrowOnError(m_Device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

		m_RootSignature->SetName(L"Root signature 1");
	}

	void RendererAPI::CreatePipelineState()
	{
		D3D12_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

		ThrowOnError(D3DReadFileToBlob(L"C:/dev/Game Engine/nino/bin/Debug/nino-sandbox/VertexShader.cso", &vertexShader));

		ThrowOnError(D3DReadFileToBlob(L"C:/dev/Game Engine/nino/bin/Debug/nino-sandbox/PixelShader.cso", &pixelShader));

		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveType;
			CD3DX12_PIPELINE_STATE_STREAM_PS PixelShader;
			CD3DX12_PIPELINE_STATE_STREAM_VS VertexShader;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		} pipelineStateStream;

		D3D12_RT_FORMAT_ARRAY rtvFormat = {};
		rtvFormat.NumRenderTargets = 1;
		rtvFormat.RTFormats[0] = { DXGI_FORMAT_R8G8B8A8_UNORM };

		pipelineStateStream.pRootSignature = m_RootSignature.Get();
		pipelineStateStream.InputLayout = { inputDesc, _countof(inputDesc) };
		pipelineStateStream.PrimitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateStream.PixelShader = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		pipelineStateStream.VertexShader = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		pipelineStateStream.DSVFormat = DXGI_FORMAT_UNKNOWN;
		pipelineStateStream.RTVFormats = rtvFormat;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStreamDesc = { sizeof(PipelineStateStream), &pipelineStateStream };

		ThrowOnError(m_Device->CreatePipelineState(&pipelineStreamDesc, IID_PPV_ARGS(&m_PSO)));
	}

	void RendererAPI::CreateViewport()
	{
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = m_Width;
		m_Viewport.Height = m_Height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_ScissorRect.left = 0;
		m_ScissorRect.right = m_Width;
		m_ScissorRect.top = 0;
		m_ScissorRect.bottom = m_Height;
	}

	void RendererAPI::CreateVertexBuffer()
	{
		Vertex vertices[3] =
		{
			{{0.5f, -0.5f, 0.0f},{1.0f, 0.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{0.0f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f}}
		};

		uint32_t indices[3] = { 0, 1, 2 };

		size_t vertexBufferSize = sizeof(vertices);
		size_t indexBufferSize = sizeof(indices);

		D3D12_HEAP_PROPERTIES vertexHeapProps = {};
		vertexHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		vertexHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		vertexHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		vertexHeapProps.CreationNodeMask = 1;
		vertexHeapProps.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC vertexDesc = {};
		vertexDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexDesc.Alignment = 0;
		vertexDesc.Width = vertexBufferSize;
		vertexDesc.Height = 1;
		vertexDesc.DepthOrArraySize = 1;
		vertexDesc.MipLevels = 1;
		vertexDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexDesc.SampleDesc.Count = 1;
		vertexDesc.SampleDesc.Quality = 0;
		vertexDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ThrowOnError(m_Device->CreateCommittedResource(&vertexHeapProps, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_VertexBuffer)));

		UINT8* pVertexData;

		D3D12_RANGE readRange = {};
		readRange.Begin = 0;
		readRange.End = 0;

		ThrowOnError(m_VertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexData)));
		memcpy(pVertexData, vertices, vertexBufferSize);
		m_VertexBuffer->Unmap(0, nullptr);

		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.SizeInBytes = vertexBufferSize;
		m_VertexBufferView.StrideInBytes = sizeof(Vertex);

		D3D12_HEAP_PROPERTIES indexHeapProps = {};
		indexHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		indexHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		indexHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		indexHeapProps.CreationNodeMask = 1;
		indexHeapProps.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC indexDesc = {};
		indexDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		indexDesc.Alignment = 0;
		indexDesc.Width = indexBufferSize;
		indexDesc.Height = 1;
		indexDesc.DepthOrArraySize = 1;
		indexDesc.MipLevels = 1;
		indexDesc.Format = DXGI_FORMAT_UNKNOWN;
		indexDesc.SampleDesc.Count = 1;
		indexDesc.SampleDesc.Quality = 0;
		indexDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		indexDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ThrowOnError(m_Device->CreateCommittedResource(&indexHeapProps, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_IndexBuffer)));

		UINT8* pIndexData;

		ThrowOnError(m_IndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexData)));
		memcpy(pIndexData, indices, indexBufferSize);
		m_IndexBuffer->Unmap(0, nullptr);

		m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_IndexBufferView.SizeInBytes = indexBufferSize;
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

	void RendererAPI::WaitForPreviousFrame()
	{
		if (m_Fences[m_CurrentBackBufferIndex]->GetCompletedValue() < m_FenceValues[m_CurrentBackBufferIndex])
		{
			ThrowOnError(m_Fences[m_CurrentBackBufferIndex]->SetEventOnCompletion(m_FenceValues[m_CurrentBackBufferIndex], m_FenceEvent));

			WaitForSingleObject(m_FenceEvent, INFINITE);
		}

		m_FenceValues[m_CurrentBackBufferIndex]++;
	}
}


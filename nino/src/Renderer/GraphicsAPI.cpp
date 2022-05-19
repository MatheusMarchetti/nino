#include "corepch.h"
#include "GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	GraphicsAPI::GraphicsAPI()
	{
		EnableDebugLayer();

		m_SelectedAdapter = m_DefaultAdapter ? QueryAdapters() : SelectedAdapter();

		DXGI_ADAPTER_DESC1 adapterDesc;
		m_SelectedAdapter->GetDesc1(&adapterDesc);
		m_SelectedAdapterName = adapterDesc.Description;

		m_Device = CreateDevice();
	}

	void GraphicsAPI::EnableDebugLayer()
	{
#ifdef CORE_DEBUG
		ComPtr<ID3D12Debug1> debugInterface;

		ThrowOnError(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

		debugInterface->EnableDebugLayer();
#endif
	}

	void GraphicsAPI::Release()
	{
		m_Device = nullptr;

		for (auto& adapter : m_AvailableAdapters)
		{
			adapter.second = nullptr;
		}

		m_SelectedAdapter = nullptr;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter4> GraphicsAPI::QueryAdapters()
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

	Microsoft::WRL::ComPtr<IDXGIAdapter4> GraphicsAPI::SelectedAdapter()
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

	Microsoft::WRL::ComPtr<ID3D12Device2> GraphicsAPI::CreateDevice()
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

	void GraphicsAPI::CreateRootSignature()
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

	void GraphicsAPI::CreatePipelineState()
	{
		D3D12_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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
}


#pragma once

#include "Core.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace nino
{
	struct Vertex
	{
		float position[3];
		float color[3];
	};

	class CORE_API RendererAPI
	{
	public:
		~RendererAPI();

		void Create(HWND& window, const uint32_t& width, const uint32_t& height);

		void Clear(float r, float g, float b, float a);
		void Render();

		void CreateRootSignature();
		void CreatePipelineState();
		void CreateViewport();

		void CreateVertexBuffer();

		void ToggleVSync(bool vsync);

	private:
		void EnableDebugLayer();
		void ReportAPIObjects();
		bool CheckTearingSupport();
		void WaitForPreviousFrame();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> QueryAdapters();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> SelectedAdapter();
		Microsoft::WRL::ComPtr<ID3D12Device2> CreateDevice();
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue();
		Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain();
		void CreateRenderTargets();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCommandList();
		void CreateFences();

	private:
		static constexpr uint32_t s_BufferCount = 3;
		HWND m_Window;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_CurrentBackBufferIndex;
		uint32_t m_RTVOffsetSize;
		uint64_t m_FenceValues[s_BufferCount];
		bool m_Tearing;
		bool m_VSync = true;
		bool m_DefaultAdapter = true;
		HANDLE m_FenceEvent;
		std::wstring m_SelectedAdapterName;

		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<IDXGIAdapter4>> m_AvailableAdapters;

		Microsoft::WRL::ComPtr<IDXGIAdapter4> m_SelectedAdapter;
		Microsoft::WRL::ComPtr<ID3D12Device2> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_DirectCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Buffers[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>m_CommandAllocators[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fences[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;

		CD3DX12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;

		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	};
}
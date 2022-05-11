#pragma once

#include "Core.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>

namespace nino
{
	class CORE_API RendererAPI
	{
	public:
		~RendererAPI();

		void Create(HWND& window, const uint32_t& width, const uint32_t& height);
		void Resize(const uint32_t& width, const uint32_t& height);
		void ToggleVSync(bool vsync);

	private:
		void EnableDebugLayer();
		bool CheckTearingSupport();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> QueryAdapters();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> SelectedAdapter();
		Microsoft::WRL::ComPtr<ID3D12Device2> CreateDevice();
		Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain();

	private:
		static constexpr uint32_t s_BufferCount = 3;
		HWND m_Window;
		HANDLE m_FenceEvent;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_CurrentBackBufferIndex;
		uint32_t m_DescriptorHeapsSizes[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
		uint64_t m_FenceValues[s_BufferCount];
		bool m_Tearing;
		bool m_VSync = true;
		bool m_DefaultAdapter = true;
		std::wstring m_SelectedAdapterName;

		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<IDXGIAdapter4>> m_AvailableAdapters;

		Microsoft::WRL::ComPtr<IDXGIAdapter4> m_SelectedAdapter;
		Microsoft::WRL::ComPtr<ID3D12Device2> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_DirectCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CopyCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_Buffers[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
	};
}
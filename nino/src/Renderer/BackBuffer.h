#pragma once

#include "Core.h"
#include "Renderer/GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
	class GraphicsContext;
	class CommandManager;
}

namespace nino
{
	class CORE_API BackBuffer
	{
	public:
		BackBuffer(uint32_t width, uint32_t height, GraphicsAPI* graphicsAPI, GraphicsContext* graphicsContext, CommandManager* commandManager);
		~BackBuffer();

		void Clear(float color[4], float depth);
		void SetViewport();
		void Present(bool vSync);

	private:
		void CreateRTVHeap(uint32_t number);
		void CreateDSVHeap(uint32_t number);

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return m_CommandList; }


	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVDescriptorHeap;
		uint32_t m_RTVOffset;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_RTVBuffers[s_BufferCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> m_DSVBuffer;
		D3D12_RESOURCE_STATES m_CurrentState;

		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;

		GraphicsAPI* m_GraphicsAPI;
		GraphicsContext* m_GraphicsContext;
		CommandManager* m_CommandManager;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& m_CommandList;
	};
}
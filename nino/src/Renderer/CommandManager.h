#pragma once

#include "Core.h"
#include "GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class CORE_API CommandManager
	{
	private:
		struct CommandEntry
		{
			uint64_t FenceValue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		};

	public:
		CommandManager(GraphicsAPI* graphicsAPI);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type);
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList(D3D12_COMMAND_LIST_TYPE type);

		void GetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		void ManageResources(D3D12_COMMAND_LIST_TYPE type, std::initializer_list<Microsoft::WRL::ComPtr<ID3D12Resource>> resources);

		void Execute(uint32_t currentFrame);
		void Signal(uint32_t currentFrame, uint32_t nextFrame);

		void Release();

	private:
		void CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type);
		void CreateCommandList(D3D12_COMMAND_LIST_TYPE type);
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator(D3D12_COMMAND_LIST_TYPE type);
		void CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type);
		void Execute(D3D12_COMMAND_LIST_TYPE type, uint32_t currentFrame);

		void Flush();

	private:
		uint32_t m_GlobalFence;
		uint32_t m_FrameFence[s_BufferCount] = {};
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, Microsoft::WRL::ComPtr<ID3D12CommandQueue>> m_CommandQueues;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::list<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>> m_AvailableCommandLists;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> m_AvailableCommandAllocators;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::list<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>> m_InFlightCommandLists;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::vector<CommandEntry>> m_InFlightCommands;
		std::vector<CommandEntry> m_RunningCommands;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;

		GraphicsAPI* m_GraphicsAPI;
	};
}
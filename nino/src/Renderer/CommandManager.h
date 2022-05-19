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

			~CommandEntry()
			{

			}
		};

	public:
		CommandManager(GraphicsAPI* graphicsAPI);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type);
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList(D3D12_COMMAND_LIST_TYPE type);

		void Execute();

		void Release();

	private:
		void CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type);
		void CreateCommandList(D3D12_COMMAND_LIST_TYPE type);
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator(D3D12_COMMAND_LIST_TYPE type);
		void CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type);
		void Execute(D3D12_COMMAND_LIST_TYPE type);

		void Flush();

	private:
		uint32_t m_GlobalFence;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, Microsoft::WRL::ComPtr<ID3D12CommandQueue>> m_CommandQueues;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::queue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>> m_AvailableCommandLists;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::queue<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> m_AvailableCommandAllocators;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>>> m_InFlightCommandLists;
		std::unordered_map<D3D12_COMMAND_LIST_TYPE, std::vector<CommandEntry>> m_InFlightCommands;
		std::vector<CommandEntry> m_RunningCommands;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;

		GraphicsAPI* m_GraphicsAPI;
	};
}
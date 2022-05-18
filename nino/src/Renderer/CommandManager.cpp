#include "corepch.h"
#include "CommandManager.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
   using namespace Microsoft::WRL;

   CommandManager::CommandManager(std::shared_ptr<GraphicsAPI>& graphicsAPI)
       : m_GlobalFence(0), m_GraphicsAPI(graphicsAPI)
   {
       auto device = m_GraphicsAPI->GetDevice();

       ThrowOnError(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
   }

   ComPtr<ID3D12CommandQueue> CommandManager::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type)
    {
       if (m_CommandQueues.count(type) == 0)
           CreateCommandQueue(type);
        
       return m_CommandQueues[type];
    }

   Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CommandManager::GetCommandList(D3D12_COMMAND_LIST_TYPE type)
   {
       if (m_CommandQueues.count(type) == 0)
           CreateCommandQueue(type);

       if (m_AvailableCommandLists[type].empty())
           CreateCommandList(type);

       auto commandlist = m_AvailableCommandLists[type].front();
       m_AvailableCommandLists[type].pop();

       auto commandAllocator = m_AvailableCommandAllocators[type].front();
       m_AvailableCommandAllocators[type].pop();

       CommandEntry entry = {};
       entry.commandAllocator = commandAllocator;

       m_InFlightCommandLists[type].push_back(commandlist);
       m_InFlightCommands[type].push_back(entry);

       return m_InFlightCommandLists[type].back();
   }

   void CommandManager::Execute()
   {
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COPY].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_COPY);
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_COMPUTE);
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_DIRECT);
   }

   void CommandManager::Execute(D3D12_COMMAND_LIST_TYPE type)
   {
       std::vector<ID3D12CommandList*> commandArray;

       for (auto& command : m_InFlightCommandLists[type]) // Final preparation to submit command lists
       {
           command->Close();
           commandArray.push_back(command.Get());
       }

       m_CommandQueues[type]->ExecuteCommandLists(commandArray.size(), commandArray.data());
       m_CommandQueues[type]->Signal(m_Fence.Get(), m_GlobalFence);

       for (auto it = m_InFlightCommands[type].begin(); it != m_InFlightCommands[type].end(); it++) // Save commands with data for proper release
       {
           it->FenceValue = m_GlobalFence;

           m_RunningCommands.push_back(*it);
       }

       m_InFlightCommands[type].clear();

       for (auto it = m_InFlightCommandLists[type].begin(); it != m_InFlightCommandLists[type].end(); it++) // Reset command lists for imediate reuse
       {
           it->Get()->Reset(GetCommandAllocator(type).Get(), nullptr);
           m_AvailableCommandLists[type].push(*it);
       }

       m_InFlightCommandLists[type].clear();

 //      auto it = m_RunningCommands.begin();
 //
 //      while (m_RunningCommands.end() - it >= m_RunningCommands.size())
 //      {
 //          if (it->FenceValue < m_Fence->GetCompletedValue())
 //          {
 //              for (auto& resource : it->resources)
 //              {
 //                  resource->Release();
 //              }
 //
 //              it->commandAllocator->Reset();
 //
 //              m_AvailableCommandAllocators[type].push(it->commandAllocator);
 //
 //              it = m_RunningCommands.erase(it);
 //          }
 //          else
 //              ++it;
 //      }

       for (auto it = m_RunningCommands.begin(); it != m_RunningCommands.end();)
       {
           if (it->FenceValue < m_Fence->GetCompletedValue())
           {
               for (auto& resource : it->resources)
               {
                   resource->Release();
               }

               it->commandAllocator->Reset();

               m_AvailableCommandAllocators[type].push(it->commandAllocator);

               it = m_RunningCommands.erase(it);
           }
           else
               ++it;
       }

       m_GlobalFence++;
   }

   void CommandManager::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type)
   {
       auto device = m_GraphicsAPI->GetDevice();

       D3D12_COMMAND_QUEUE_DESC pDesc = {};
       pDesc.Type = type;
       pDesc.NodeMask = 0;
       pDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
       pDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

       ComPtr<ID3D12CommandQueue> commandQueue;

       ThrowOnError(device->CreateCommandQueue(&pDesc, IID_PPV_ARGS(&commandQueue)));

       m_CommandQueues[type] = commandQueue;
   }

   void CommandManager::CreateCommandList(D3D12_COMMAND_LIST_TYPE type)
   {
       auto device = m_GraphicsAPI->GetDevice();
       ComPtr<ID3D12GraphicsCommandList> commandList;

       ThrowOnError(device->CreateCommandList(0, type, GetCommandAllocator(type).Get(), nullptr, IID_PPV_ARGS(&commandList)));

       m_AvailableCommandLists[type].push(commandList);
   }

   Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandManager::GetCommandAllocator(D3D12_COMMAND_LIST_TYPE type)
   {
       if (m_AvailableCommandAllocators[type].empty())
           CreateCommandAllocator(type);
       
       return m_AvailableCommandAllocators[type].front();
   }

   void CommandManager::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type)
   {
       auto device = m_GraphicsAPI->GetDevice();
       ComPtr<ID3D12CommandAllocator> allocator;

       ThrowOnError(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator)));

       m_AvailableCommandAllocators[type].push(allocator);
   }
}


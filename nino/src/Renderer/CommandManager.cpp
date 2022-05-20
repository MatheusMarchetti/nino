#include "corepch.h"
#include "CommandManager.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
   using namespace Microsoft::WRL;

   CommandManager::CommandManager(GraphicsAPI* graphicsAPI)
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
        
       GetCommandList(*m_AvailableCommandLists[type].begin(), type);

       return m_InFlightCommandLists[type].back();
   }

   void CommandManager::GetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_COMMAND_LIST_TYPE type)
   {
       if (!m_AvailableCommandLists[type].empty())
       {
           for (auto it = m_AvailableCommandLists[type].begin(); it != m_AvailableCommandLists[type].end();)
           {
               if (commandList == *it)
               {
                   m_InFlightCommandLists[type].splice(m_InFlightCommandLists[type].end(), m_AvailableCommandLists[type], it);

                   CommandEntry entry = {};
                   entry.commandAllocator = m_AvailableCommandAllocators[type].front();

                   m_InFlightCommands[type].push_back(entry);

                   m_AvailableCommandAllocators[type].erase(m_AvailableCommandAllocators[type].begin());

                   it = m_AvailableCommandLists[type].begin();
               }
               else
                   ++it;
           }
       }
   }

   void CommandManager::ManageResources(D3D12_COMMAND_LIST_TYPE type, std::initializer_list<Microsoft::WRL::ComPtr<ID3D12Resource>> resources)
   {
       for (auto& command : m_InFlightCommands[type])
       {
           if (command.resources.empty() && command.FenceValue == 0) // Sanity check on fence value. It should always be 0 on InFlightCommands
           {
               command.resources = { resources };

               return;
           }
       }
   }

   void CommandManager::Execute(uint32_t currentFrame)
   {
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COPY].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_COPY, currentFrame);
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_DIRECT, currentFrame);
       if (!m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE].empty())
           Execute(D3D12_COMMAND_LIST_TYPE_COMPUTE, currentFrame);
   }

   void CommandManager::Signal(uint32_t currentFrame, uint32_t nextFrame)
   {
       m_GlobalFence++;
       m_CommandQueues[D3D12_COMMAND_LIST_TYPE_DIRECT]->Signal(m_Fence.Get(), m_GlobalFence);
       m_FrameFence[currentFrame] = m_GlobalFence;

       auto it = m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].begin();
       auto size = m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].size();

       for (auto it = m_RunningCommands.begin(); it != m_RunningCommands.end();)
       {
           if (it->FenceValue < m_Fence->GetCompletedValue())
           {
               for (auto& resource : it->resources)
               {
                   resource = nullptr;
               }

               it->commandAllocator->Reset();

               m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_DIRECT].push_back(it->commandAllocator);

               it = m_RunningCommands.erase(it);
           }
           else
               ++it;
       }

       while (m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_DIRECT].size() < size) // If, for some reason, GPU gets way behind work, create additional command allocators to keep the work flow.
           CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

       for (size_t i = 0; i < size; i++) // Reset command lists for imediate reuse
       {
           it->Get()->Reset(m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_DIRECT][i].Get(), nullptr);
           m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].splice(m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].end(), m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT], it);

           it = m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].begin();
       }
   }


   void CommandManager::Execute(D3D12_COMMAND_LIST_TYPE type, uint32_t currentFrame)
   {
       std::vector<ID3D12CommandList*> commandArray;

       for (auto& command : m_InFlightCommandLists[type]) // Final preparation to submit command lists
       {
           command->Close();
           commandArray.push_back(command.Get());
       }

       m_CommandQueues[type]->ExecuteCommandLists(commandArray.size(), commandArray.data());

       for (auto it = m_InFlightCommands[type].begin(); it != m_InFlightCommands[type].end(); it++) // Save commands with data for proper release
       {
           it->FenceValue = m_FrameFence[currentFrame];

           m_RunningCommands.push_back(*it);
       }

       m_InFlightCommands[type].clear();
   }

   void CommandManager::Flush()
   {
       for (auto it = m_RunningCommands.begin(); it != m_RunningCommands.end();)
       {
           if (m_Fence->GetCompletedValue() < it->FenceValue)
           {
               HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

               ThrowOnError(m_Fence->SetEventOnCompletion(it->FenceValue, eventHandle));

               WaitForSingleObject(eventHandle, INFINITE);

               it->commandAllocator = nullptr;

               for (auto& resource : it->resources)
               {
                   resource = nullptr;
               }

               CloseHandle(eventHandle);

               it = m_RunningCommands.erase(it);
           }
           else
               ++it;
       }
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

       m_AvailableCommandLists[type].push_back(commandList);
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

       m_AvailableCommandAllocators[type].push_back(allocator);
   }

   void CommandManager::Release()
   {
       Flush();

       for (auto& commands : m_InFlightCommands[D3D12_COMMAND_LIST_TYPE_COPY])
       {
           commands.commandAllocator = nullptr;

           for (auto& resource : commands.resources)
           {
               resource = nullptr;
           }
       }

       for (auto& commands : m_InFlightCommands[D3D12_COMMAND_LIST_TYPE_COMPUTE])
       {
           commands.commandAllocator = nullptr;

           for (auto& resource : commands.resources)
           {
               resource = nullptr;
           }
       }

       for (auto& commands : m_InFlightCommands[D3D12_COMMAND_LIST_TYPE_DIRECT])
       {
           commands.commandAllocator = nullptr;

           for (auto& resource : commands.resources)
           {
               resource = nullptr;
           }
       }

       for (auto& lists : m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COPY])
       {
           lists = nullptr;
       }

       for (auto& lists : m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE])
       {
           lists = nullptr;
       }

       for (auto& lists : m_InFlightCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT])
       {
           lists = nullptr;
       }

       for (auto& allocator : m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_COPY])
       {
           allocator = nullptr;
       }

       for (auto& allocator : m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_COMPUTE])
       {
           allocator = nullptr;
       }

       for (auto& allocator : m_AvailableCommandAllocators[D3D12_COMMAND_LIST_TYPE_DIRECT])
       {
           allocator = nullptr;
       }
       
       while (!m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COPY].empty())
       {
           auto& list = m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COPY].front();
           list = nullptr;

           m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COPY].pop_front();
       }

       while (!m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE].empty())
       {
           auto& list = m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE].front();
           list = nullptr;

           m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_COMPUTE].pop_front();
       }

       while (!m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].empty())
       {
           auto& list = m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].front();
           list = nullptr;

           m_AvailableCommandLists[D3D12_COMMAND_LIST_TYPE_DIRECT].pop_front();
       }

       m_CommandQueues[D3D12_COMMAND_LIST_TYPE_COPY] = nullptr;
       m_CommandQueues[D3D12_COMMAND_LIST_TYPE_COMPUTE] = nullptr;
       m_CommandQueues[D3D12_COMMAND_LIST_TYPE_DIRECT] = nullptr;

       m_Fence = nullptr;
   }
}


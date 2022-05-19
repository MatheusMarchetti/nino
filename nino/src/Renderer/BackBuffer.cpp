#include "corepch.h"
#include "BackBuffer.h"

#include "Renderer/GraphicsAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/CommandManager.h"

namespace nino
{
	using namespace Microsoft::WRL;

	BackBuffer::BackBuffer(uint32_t width, uint32_t height, GraphicsAPI* graphicsAPI, GraphicsContext* graphicsContext, CommandManager* commandManager)
		:m_GraphicsAPI(graphicsAPI), m_GraphicsContext(graphicsContext), m_CommandManager(commandManager), m_CommandList(m_CommandManager->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT))
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto swapchain = m_GraphicsContext->GetContext();

		CreateRTVHeap(s_BufferCount);
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		m_RTVOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (uint32_t i = 0; i < s_BufferCount; i++)
		{
			ThrowOnError(swapchain->GetBuffer(i, IID_PPV_ARGS(&m_RTVBuffers[i])));
			device->CreateRenderTargetView(m_RTVBuffers[i].Get(), nullptr, RTVHandle);


			RTVHandle.Offset(m_RTVOffset);
		}

		m_CurrentState = D3D12_RESOURCE_STATE_PRESENT;

		CreateDSVHeap(1);

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0.0f;

		ThrowOnError(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, 
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&m_DSVBuffer)));

		CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle(m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		device->CreateDepthStencilView(m_DSVBuffer.Get(), nullptr, DSVHandle);

		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_ScissorRect.left = 0;
		m_ScissorRect.right = width;
		m_ScissorRect.top = 0;
		m_ScissorRect.bottom = height;
	}

	void BackBuffer::Clear(float color[4], float depth)
	{
		if(!m_CommandList)
			m_CommandList = m_CommandManager->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

		auto swapchain = m_GraphicsContext->GetContext();

		uint32_t backBuffer = swapchain->GetCurrentBackBufferIndex();

		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle(m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		RTVHandle.ptr += backBuffer * m_RTVOffset;

		if (m_CurrentState != D3D12_RESOURCE_STATE_RENDER_TARGET)
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RTVBuffers[backBuffer].Get(), m_CurrentState, D3D12_RESOURCE_STATE_RENDER_TARGET);

			m_CommandList->ResourceBarrier(1, &barrier);
			m_CurrentState = D3D12_RESOURCE_STATE_RENDER_TARGET;
		}

		m_CommandList->ClearRenderTargetView(RTVHandle, color, 0, nullptr);
		m_CommandList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, nullptr);
	}

	void BackBuffer::SetViewport()
	{
		if (!m_CommandList)
			m_CommandList = m_CommandManager->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
		m_CommandList->RSSetViewports(1, &m_Viewport);
	}

	void BackBuffer::Present(bool vSync)
	{
		if (!m_CommandList)
			m_CommandList = m_CommandManager->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

		auto swapchain = m_GraphicsContext->GetContext();

		uint32_t backBuffer = swapchain->GetCurrentBackBufferIndex();

		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle(m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		RTVHandle.ptr += backBuffer * m_RTVOffset;

		m_CommandList->OMSetRenderTargets(1, &RTVHandle, FALSE, &DSVHandle);

		if (m_CurrentState != D3D12_RESOURCE_STATE_PRESENT)
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RTVBuffers[backBuffer].Get(), m_CurrentState, D3D12_RESOURCE_STATE_PRESENT);

			m_CommandList->ResourceBarrier(1, &barrier);
			m_CurrentState = D3D12_RESOURCE_STATE_PRESENT;
		}

		m_CommandManager->Execute();

		swapchain->Present(vSync ? 1 : 0, 0);
	}

	void BackBuffer::Release()
	{
		m_DSVBuffer = nullptr;
		m_DSVDescriptorHeap = nullptr;

		for (int i = 0; i < s_BufferCount; i++)
		{
			m_RTVBuffers[i] = nullptr;
		}

		m_RTVDescriptorHeap = nullptr;
	}

	void BackBuffer::CreateRTVHeap(uint32_t number)
	{
		auto device = m_GraphicsAPI->GetDevice();

		D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc = {};
		descriptorDesc.NodeMask = 0;
		descriptorDesc.NumDescriptors = number;
		descriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowOnError(device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap)));
	}

	void BackBuffer::CreateDSVHeap(uint32_t number)
	{
		auto device = m_GraphicsAPI->GetDevice();

		D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc = {};
		descriptorDesc.NodeMask = 0;
		descriptorDesc.NumDescriptors = number;
		descriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		descriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowOnError(device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(&m_DSVDescriptorHeap)));
	}
}


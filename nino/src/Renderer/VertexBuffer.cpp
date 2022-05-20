#include "corepch.h"
#include "VertexBuffer.h"

#include "Renderer/GraphicsAPI.h"
#include "Renderer/CommandManager.h"

namespace nino
{
	VertexBuffer::VertexBuffer(GraphicsAPI* graphicsAPI, CommandManager* commandManager)
		:m_GraphicsAPI(graphicsAPI), m_CommandManager(commandManager), m_CommandList(m_CommandManager->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT))
	{
		m_CommandList->SetName(L"Vertex buffer command list");
	}

	void VertexBuffer::UploadBuffer()
	{
		m_CommandManager->GetCommandList(m_CommandList);

		auto device = m_GraphicsAPI->GetDevice();

		Vertex vertices[] =
		{
			{ {0.0f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
			{ {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
			{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} }
		};

		size_t bufferSize = sizeof(vertices);

		ThrowOnError(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_VertexBuffer)));

		m_VertexBuffer->SetName(L"Vertex Buffer");

		ThrowOnError(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_UploadBuffer)));

		m_VertexBuffer->SetName(L"Upload Buffer");

		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(vertices);
		vertexData.RowPitch = bufferSize;
		vertexData.SlicePitch = bufferSize;

		UpdateSubresources(m_CommandList.Get(), m_VertexBuffer.Get(), m_UploadBuffer.Get(), 0, 0, 1, &vertexData);

		m_CommandManager->ManageResources(D3D12_COMMAND_LIST_TYPE_DIRECT, { m_UploadBuffer });

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		m_CommandList->ResourceBarrier(1, &barrier);

		D3D12_VERTEX_BUFFER_VIEW vertexView = {};
		vertexView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(Vertex);
		vertexView.SizeInBytes = bufferSize;

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &vertexView);
		m_CommandList->SetGraphicsRootSignature(m_GraphicsAPI->GetRootSignature().Get());
		m_CommandList->SetPipelineState(m_GraphicsAPI->GetPipelineState().Get());
		m_CommandList->DrawInstanced(3, 1, 0, 0);
	}

	void VertexBuffer::Release()
	{
		m_UploadBuffer = nullptr;
		m_VertexBuffer = nullptr;
	}
}

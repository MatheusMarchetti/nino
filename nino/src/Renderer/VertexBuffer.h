#pragma once

#include "Core.h"
#include "Renderer/GraphicsCore.h"

#include "NinoMath.h"

namespace nino
{
	class GraphicsAPI;
	class CommandManager;
}

namespace nino
{
	class CORE_API VertexBuffer
	{
		struct Vertex
		{
			vec3f Position;
			vec4f Color;
		};

	public:
		VertexBuffer(GraphicsAPI* graphicsAPI, CommandManager* commandManager);

		void UploadBuffer();

		void Release();

	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>*> m_Resources;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;

		GraphicsAPI* m_GraphicsAPI;
		CommandManager* m_CommandManager;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& m_CommandList;
	};
}
#pragma once

#include "Core.h"
#include "GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class CORE_API IndexBuffer
	{
	public:
		IndexBuffer(GraphicsAPI* graphicsAPI);
		~IndexBuffer() {}

		void SetIndexBuffer(PrimitiveType primitive);

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

		DWORD m_TriangleIndices[3] = { 0, 1, 2 };
		DWORD m_QuadIndices[6] = { 0, 1, 2, 0, 2, 3 };

		GraphicsAPI* m_GraphicsAPI;
	};
}
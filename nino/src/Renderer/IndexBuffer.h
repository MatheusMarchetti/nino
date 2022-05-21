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
		DWORD m_CubeIndices[36] =
		{
				0, 1, 2, 0, 2, 3,
				4, 6, 5, 4, 7, 6,
				4, 5, 1, 4, 1, 0,
				3, 2, 6, 3, 6, 7,
				1, 5, 6, 1, 6, 2,
				4, 0, 3, 4, 3, 7
		};

		GraphicsAPI* m_GraphicsAPI;
	};
}
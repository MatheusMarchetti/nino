#pragma once

#include "Core.h"
#include "GraphicsCore.h"

#include "NinoMath.h"

namespace nino
{
	class GraphicsAPI;
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
		VertexBuffer(GraphicsAPI* graphicsAPI);
		~VertexBuffer() {}

		void SetVertexBuffer(vec4f color, PrimitiveType primitive);

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;

		GraphicsAPI* m_GraphicsAPI;
	};
}
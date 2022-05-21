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

		void SetVertexBuffer(PrimitiveType primitive);

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;

		Vertex m_Triangle[3] = {
			{{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{0.5f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
		};
		Vertex m_Quad[4] = {
			{{-0.5f, 0.5f, 0.0f}, {0.5f, 0.0f, 0.0f, 1.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.5f, 0.0f, 1.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.5f, 1.0f}},
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f, 1.0f}},
		};

		GraphicsAPI* m_GraphicsAPI;
	};
}
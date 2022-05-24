#pragma once

#include "Renderer/GraphicsCore.h"

#include "Core/NinoMath.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class VertexBuffer
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
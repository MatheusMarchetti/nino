#pragma once

#include "Core.h"
#include "GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class CORE_API RenderTargets
	{
	public:
		RenderTargets(GraphicsAPI* graphicsAPI);
		~RenderTargets() {}

		void SetRenderTargets();
		void SetViewports(uint32_t width, uint32_t height);
		void Draw(uint32_t numIndices);
		void Clear(float color[4], float depth = 1.0f);
		void Present(bool vSync);

		void Release();

	private:
		void CreateRenderTargetView();

	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTarget;

		GraphicsAPI* m_GraphicsAPI;
	};
}
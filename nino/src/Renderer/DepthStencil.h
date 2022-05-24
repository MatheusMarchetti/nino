#pragma once

#include "Renderer/GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class DepthStencil
	{
	public:
		DepthStencil(uint32_t width, uint32_t height, GraphicsAPI* graphicsAPI);
		~DepthStencil() {}

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDSV() { return m_DepthStencilView; }

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

		GraphicsAPI* m_GraphicsAPI;
	};
}

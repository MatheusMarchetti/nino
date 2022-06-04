#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Window;
}

namespace nino
{
	class GraphicsAPI
	{
	public:
		~GraphicsAPI();

		static void Init(Window* window);

		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_Device; }
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_DeviceContext; }
		static Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() { return m_SwapChain; }

		static void SetViewport(uint32_t width, uint32_t height, uint32_t topX, uint32_t topY);
		static void Clear(float color[4], float depth);
		static void Present(bool vSync);

	private:
		static BOOL m_TearingSupport;
		static D3D11_VIEWPORT m_Viewport;
		static Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		static Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
		static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	};
}
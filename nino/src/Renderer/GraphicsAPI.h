#pragma once

#include "Core.h"
#include "GraphicsCore.h"

namespace nino
{
	class Window;
}

namespace nino
{
	class CORE_API GraphicsAPI
	{
	public:
		GraphicsAPI(Window* window);
		~GraphicsAPI() {}

		Microsoft::WRL::ComPtr<ID3D11Device5> GetDevice() { return m_Device; }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> GetContext() { return m_DeviceContext; }
		Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() { return m_SwapChain; }

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device5> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
	};
}
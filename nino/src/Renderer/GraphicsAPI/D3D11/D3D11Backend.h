#pragma once

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/GraphicsAPI/GraphicsCore.h"

#include <d3d11_4.h>

namespace nino
{
	class Window;

	class D3D11Backend : public GraphicsAPI
	{
	public:
		D3D11Backend();
		virtual ~D3D11Backend() = default;

		virtual void CreateSwapChain(Window* window) override;

		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_Device; }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_DeviceContext; }

		virtual void Present(bool vSync) override;

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetAdapter();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	};
}
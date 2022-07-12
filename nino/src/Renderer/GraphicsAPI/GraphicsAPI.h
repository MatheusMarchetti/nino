#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Framebuffer.h"

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

		static void CreateDeviceAndContext();
		static void CreateSwapChain(Window* window);

		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return s_Device; }
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return s_DeviceContext; }
		static Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain(Window* window);

		static void BindFramebuffers(const std::vector<Ref<Framebuffer>>& framebuffers);
		static void Present(bool vSync);

	private:
		static BOOL s_TearingSupport;
		static Microsoft::WRL::ComPtr<ID3D11Device> s_Device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> s_DeviceContext;
		static std::unordered_map<HWND, Microsoft::WRL::ComPtr<IDXGISwapChain4>> s_RenderTargets;
		static std::unordered_map<const char*, Framebuffer*> s_Framebuffers;
	};
}
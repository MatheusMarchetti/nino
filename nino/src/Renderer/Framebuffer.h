#pragma once

#include "Core/Core.h"
#include "Renderer/GraphicsAPI/GraphicsCore.h"

#include "Renderer/Texture.h"

namespace nino
{
	struct FramebufferDescriptor
	{
		std::string Name = "";
		Ref<Texture> ColorResource = nullptr;
		Ref<Texture> DepthStencilResource = nullptr;
		Viewport ViewportSpecification;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferDescriptor& descriptor);

		void Clear(const float* color, float depth, uint8_t stencil = 0);

		ID3D11RenderTargetView* GetRenderTarget() { return m_RenderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencil() { return m_DepthStencilView.Get(); }

		const D3D11_VIEWPORT& GetViewport() { return m_Viewport; }
		const FramebufferDescriptor& GetDescriptor() const { return m_Descriptor; }

	private:
		FramebufferDescriptor m_Descriptor;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		D3D11_VIEWPORT m_Viewport;
	};
}
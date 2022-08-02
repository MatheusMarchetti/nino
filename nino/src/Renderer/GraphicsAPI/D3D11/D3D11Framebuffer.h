#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"

#include "Renderer/Framebuffer.h"

namespace nino
{
	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(const FramebufferDescriptor& descriptor);

		virtual void Clear(const Color& color, float depth, uint8_t stencil = 0) override;
		virtual void ClearColor(const Color& color) override;
		virtual void ClearDepthStencil(float depth, uint8_t stencil = 0) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		D3D11_VIEWPORT m_Viewport;

	};
}
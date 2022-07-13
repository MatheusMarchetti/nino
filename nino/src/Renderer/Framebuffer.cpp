#include "corepch.h"
#include "Framebuffer.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	Framebuffer::Framebuffer(const FramebufferDescriptor& descriptor)
		: m_Descriptor(descriptor)
	{
		auto device = GraphicsAPI::GetDevice();
		auto context = GraphicsAPI::GetContext();

		if (m_Descriptor.ColorResource)
		{
			ThrowOnError(device->CreateRenderTargetView(m_Descriptor.ColorResource->GetResource(), nullptr, &m_RenderTargetView));
		}
		else
		{
			m_RenderTargetView = nullptr;
		}
		if(m_Descriptor.DepthStencilResource)
		{
			ThrowOnError(device->CreateDepthStencilView(m_Descriptor.DepthStencilResource->GetResource(), nullptr, &m_DepthStencilView));
		}
		else
		{
			m_DepthStencilView = nullptr;
		}

		m_Viewport.Width = m_Descriptor.ViewportSpecification.width;
		m_Viewport.Height = m_Descriptor.ViewportSpecification.height;
		m_Viewport.MinDepth = m_Descriptor.ViewportSpecification.minDepth;
		m_Viewport.MaxDepth = m_Descriptor.ViewportSpecification.maxDepth;
		m_Viewport.TopLeftX = m_Descriptor.ViewportSpecification.x;
		m_Viewport.TopLeftY = m_Descriptor.ViewportSpecification.y;
	}

	void Framebuffer::Clear(const float* color, float depth, uint8_t stencil)
	{
		auto context = GraphicsAPI::GetContext();

		context->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}
}
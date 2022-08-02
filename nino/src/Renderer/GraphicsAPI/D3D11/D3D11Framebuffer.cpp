#include "corepch.h"
#include "D3D11Framebuffer.h"

#include "Renderer/GraphicsAPI/D3D11/D3D11Texture.h"

namespace nino
{
	D3D11Framebuffer::D3D11Framebuffer(const FramebufferDescriptor& descriptor)
		: Framebuffer(descriptor)
	{
		auto device = D3D11Backend::GetDevice();

		if (m_Descriptor.ColorResource)
		{
			auto colortexture = dynamic_cast<D3D11Texture*>(m_Descriptor.ColorResource.get());
			ThrowOnError(device->CreateRenderTargetView(colortexture->GetResource(), nullptr, &m_RenderTargetView));
		}
		else
		{
			m_RenderTargetView = nullptr;
		}
		if (m_Descriptor.DepthStencilResource)
		{
			auto depthtexture = dynamic_cast<D3D11Texture*>(m_Descriptor.DepthStencilResource.get());
			D3D11_TEXTURE2D_DESC textureDesc;
			depthtexture->GetResource()->GetDesc(&textureDesc);

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = textureDesc.Format == DXGI_FORMAT_R32_TYPELESS ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			ThrowOnError(device->CreateDepthStencilView(depthtexture->GetResource(), &dsvDesc, &m_DepthStencilView));
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

	void D3D11Framebuffer::Clear(const Color& color, float depth, uint8_t stencil)
	{
		ClearColor(color);
		ClearDepthStencil(depth, stencil);
	}

	void D3D11Framebuffer::ClearColor(const Color& color)
	{
		auto context = D3D11Backend::GetContext();

		context->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	}

	void D3D11Framebuffer::ClearDepthStencil(float depth, uint8_t stencil)
	{
		auto context = D3D11Backend::GetContext();

		context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}
}


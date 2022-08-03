#pragma once

#include "Core/Core.h"

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
		Framebuffer(const FramebufferDescriptor& descriptor) : m_Descriptor(descriptor) {}
		virtual ~Framebuffer() = default;

		virtual void Clear(const Color& color, float depth, uint8_t stencil = 0) = 0;
		virtual void ClearColor(const Color& color) = 0;
		virtual void ClearDepthStencil(float depth, uint8_t stencil = 0) = 0;

		const FramebufferDescriptor& GetDescriptor() const { return m_Descriptor; }

		static Ref<Framebuffer> Create(const FramebufferDescriptor& descriptor);

	protected:
		FramebufferDescriptor m_Descriptor;
	};
}
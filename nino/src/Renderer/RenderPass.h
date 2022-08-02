#pragma once

#include "Core/Core.h"

#include "Renderer/Framebuffer.h"

namespace nino
{
	struct PassDescriptor
	{
		std::string Name;
	};

	class RenderPass
	{
	public:
		RenderPass(const PassDescriptor& descriptor);
		virtual ~RenderPass() = default;

		PassDescriptor& GetDescriptor() { return m_Descriptor; }

	protected:
		PassDescriptor m_Descriptor;
		Ref<Framebuffer> m_FrameBuffer;
	};
}
#pragma once

#include "Core/Core.h"

#include "Renderer/RenderPass.h"

namespace nino
{
	class HDRTonemap : public RenderPass
	{
	public:
		HDRTonemap(std::initializer_list<Ref<Framebuffer>> framebuffers);
		virtual ~HDRTonemap() = default;

	private:

	};
}
#pragma once

#include "Core/Core.h"

#include "Renderer/RenderPass.h"

namespace nino
{
	class DepthPrepass : public RenderPass
	{
	public:
		DepthPrepass(std::initializer_list<Ref<Framebuffer>> framebuffers);
		virtual ~DepthPrepass() = default;

	private:

	};
}
#pragma once

#include "Core/Core.h"

#include "Renderer/RenderPass.h"

namespace nino
{
	class LightCulling : public RenderPass
	{
	public:
		LightCulling(std::initializer_list<Ref<Framebuffer>> framebuffers);
		virtual ~LightCulling() = default;

	private:

	};
}
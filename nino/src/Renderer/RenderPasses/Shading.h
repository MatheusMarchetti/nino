#pragma once

#include "Core/Core.h"

#include "Renderer/RenderPass.h"

namespace nino
{
	class Shading : public RenderPass
	{
	public:
		Shading(std::initializer_list<Ref<Framebuffer>> framebuffers);
		virtual ~Shading() = default;

	private:

	};
}
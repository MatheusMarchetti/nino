#include "corepch.h"
#include "Shading.h"

namespace nino
{
	nino::Shading::Shading(std::initializer_list<Ref<Framebuffer>> framebuffers)
		: RenderPass(framebuffers)
	{
	}
}

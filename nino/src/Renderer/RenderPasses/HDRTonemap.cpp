#include "corepch.h"
#include "HDRTonemap.h"

namespace nino
{
	HDRTonemap::HDRTonemap(std::initializer_list<Ref<Framebuffer>> framebuffers)
		: RenderPass(framebuffers)
	{
	}
}

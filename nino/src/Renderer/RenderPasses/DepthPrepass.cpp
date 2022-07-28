#include "corepch.h"
#include "DepthPrepass.h"


namespace nino
{
	DepthPrepass::DepthPrepass(std::initializer_list<Ref<Framebuffer>> framebuffers)
		: RenderPass(framebuffers)
	{

	}
}

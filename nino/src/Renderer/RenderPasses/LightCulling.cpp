#include "corepch.h"
#include "LightCulling.h"


namespace nino
{
	LightCulling::LightCulling(std::initializer_list<Ref<Framebuffer>> framebuffers)
		: RenderPass(framebuffers)
	{
	}
}

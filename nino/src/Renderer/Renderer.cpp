#include "corepch.h"
#include "Renderer.h"

#include "Core/Core.h"
#include "Core/Window.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	using namespace DirectX;

	struct RendererData
	{

	};

	static RendererData s_Data;
	bool Renderer::s_VSync = false;

	void Renderer::Init(Window* window)
	{
		GraphicsAPI::Init(window);

		NINO_CORE_INFO("Renderer subsystem initialized!");
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height, uint32_t topX, uint32_t topY)
	{
		GraphicsAPI::SetViewport(width, height, topX, topY);
	}

	void Renderer::Clear(float color[4], float depth)
	{
		GraphicsAPI::Clear(color, depth);
	}

	void Renderer::BeginScene(Ref<Scene>& scene)
	{
		// Send everything that should be updated per frame to the pipeline
	}

	void Renderer::EndScene()
	{
		GraphicsAPI::Present(s_VSync);
	}
}


#include "corepch.h"
#include "Renderer.h"

#include "Core/Core.h"
#include "Core/Window.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Bindable/BindableCore.h"
#include "Renderer/Camera.h"
#include "Renderer/Skylight.h"

namespace nino
{
	using namespace DirectX;

	struct RendererData
	{
		float AspectRatio;
		Scope<VertexConstantBuffer<SceneBuffer>> SceneConstantBuffer;
	};

	static RendererData s_Data;
	bool Renderer::s_VSync = false;

	void Renderer::Init(Window* window)
	{
		GraphicsAPI::Init(window);

		s_Data.AspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
		s_Data.SceneConstantBuffer = CreateScope< VertexConstantBuffer<SceneBuffer>>();

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

	void Renderer::BeginScene(const Camera& camera, const Skylight& skylight)
	{
		SceneBuffer sceneCB;

		sceneCB.ViewProjection = camera.GetViewProjection();
		sceneCB.EyePosition = camera.GetFocalPoint();

		s_Data.SceneConstantBuffer->Update(sceneCB);
		s_Data.SceneConstantBuffer->Bind(0);
	}

	void Renderer::EndScenes()
	{
		GraphicsAPI::Present(s_VSync);
	}
}


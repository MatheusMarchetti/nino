#include "corepch.h"
#include "Renderer.h"

#include "Window.h"

#include "Renderer/BackBuffer.h"

namespace nino
{
	struct RendererData
	{
		std::shared_ptr<BackBuffer> BackBuffers;
	};

	static RendererData s_Data;
	bool Renderer::s_VSync = false;

	Renderer::Renderer(Window* window)
		: m_CommandManager(&m_GraphicsAPI), m_GraphicsContext(window->GetWindow(), window->GetWidth(), window->GetHeight(), &m_CommandManager)
	{
		m_AspectRatio = (float)window->GetWidth() / (float)window->GetHeight();

		s_Data.BackBuffers = std::make_shared<BackBuffer>(window->GetWidth(), window->GetHeight(), &m_GraphicsAPI, &m_GraphicsContext, &m_CommandManager);

		NINO_CORE_INFO(L"Renderer subsystem initialized!");
	}

	void Renderer::Clear(float color[4], float depth)
	{
		s_Data.BackBuffers->Clear(color, depth);
	}

	void Renderer::Draw()
	{
		s_Data.BackBuffers->SetViewport();
		s_Data.BackBuffers->Present(s_VSync);
	}

	Renderer::~Renderer()
	{
		m_CommandManager.Release();
		s_Data.BackBuffers->Release();
		m_GraphicsContext.Release();
		m_GraphicsAPI.Release();
	}
}


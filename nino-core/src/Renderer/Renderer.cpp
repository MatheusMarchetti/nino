#include "corepch.h"
#include "Renderer.h"

namespace nino
{
	void Renderer::Create(HWND& window, const uint32_t& width, const uint32_t& height)
	{
		m_Width = width;
		m_Height = height;
		m_aspectRatio = m_Width / m_Height;

		m_RendererAPI.Create(window, m_Width, m_Height);

		NINO_CORE_INFO(L"Renderer subsystem initialized.");
	}

	void Renderer::Resize(const uint32_t& width, const uint32_t& height)
	{
		m_Width = std::max(1u, width);
		m_Height = std::max(1u, height);
		m_aspectRatio = m_Width / m_Height;

		m_RendererAPI.Resize(width, height);
	}

	void Renderer::ToggleVSync(bool vsync)
	{
		m_RendererAPI.ToggleVSync(vsync);
	}
}


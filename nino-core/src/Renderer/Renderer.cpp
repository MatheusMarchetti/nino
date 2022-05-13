#include "corepch.h"
#include "Renderer.h"

namespace nino
{

	void Renderer::Create(HWND& window, const uint32_t& width, const uint32_t& height)
	{
		m_Width = width;
		m_Height = height;
		m_aspectRatio = m_Width / m_Height;

		m_RendererAPI.Create(window, width, height);

		m_RendererAPI.CreateRootSignature();
		m_RendererAPI.CreatePipelineState();
		m_RendererAPI.CreateViewport();

		NINO_CORE_INFO(L"Renderer subsystem initialized.");
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		m_RendererAPI.Clear(r, g, b, a);
	}

	void Renderer::SubmitVertices(const void* vertices, size_t numElements, size_t ElementSize)
	{
		m_RendererAPI.CreateVertexBuffer(vertices, numElements, ElementSize);
	}

	void Renderer::Render()
	{
		m_RendererAPI.Render();
	}

	void Renderer::ToggleVSync(bool vsync)
	{
		m_RendererAPI.ToggleVSync(vsync);
	}
}


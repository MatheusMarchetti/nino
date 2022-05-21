#include "corepch.h"
#include "Renderer.h"

#include "Window.h"

#include "Renderer/RenderTargets.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

namespace nino
{
	struct RendererData
	{
		std::shared_ptr<RenderTargets> RenderTargets;
		std::shared_ptr<VertexBuffer> VertexBuffer;
		std::shared_ptr<IndexBuffer> IndexBuffer;
		std::shared_ptr<Shader> Shader;
	};

	static RendererData s_Data;
	bool Renderer::s_VSync = false;

	Renderer::Renderer(Window* window)
		: m_GraphicsAPI(window)
	{
		m_AspectRatio = (float)window->GetWidth() / (float)window->GetHeight();

		s_Data.RenderTargets = std::make_shared<RenderTargets>(&m_GraphicsAPI);
		s_Data.VertexBuffer = std::make_shared<VertexBuffer>(&m_GraphicsAPI);
		s_Data.IndexBuffer = std::make_shared<IndexBuffer>(&m_GraphicsAPI);
		s_Data.Shader = std::make_shared<Shader>(L"", &m_GraphicsAPI);

		s_Data.RenderTargets->SetViewports(window->GetWidth(), window->GetHeight());
		s_Data.Shader->SetLayout();

		NINO_CORE_INFO(L"Renderer subsystem initialized!");
	}

	void Renderer::SetTargets()
	{
		s_Data.RenderTargets->SetRenderTargets();
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		s_Data.RenderTargets->SetViewports(width, height);
	}

	void Renderer::Clear(float color[4], float depth)
	{
		s_Data.RenderTargets->Clear(color, depth);
	}

	void Renderer::DrawTriangle()
	{
		DrawPrimitive(PrimitiveType::TRIANGLE);
	}

	void Renderer::DrawQuad()
	{
		DrawPrimitive(PrimitiveType::QUAD);
	}


	void Renderer::Draw()
	{
		s_Data.RenderTargets->Present(s_VSync);
	}

	void Renderer::DrawPrimitive(PrimitiveType primitive)
	{
		s_Data.VertexBuffer->SetVertexBuffer(primitive);
		s_Data.IndexBuffer->SetIndexBuffer(primitive);

		switch (primitive)
		{
		case PrimitiveType::TRIANGLE:
		{
			s_Data.RenderTargets->Draw(3);

			break;
		}

		case PrimitiveType::QUAD:
		{
			s_Data.RenderTargets->Draw(6);

			break;
		}
		}
	}

	Renderer::~Renderer()
	{
		s_Data.IndexBuffer->Release();
		s_Data.VertexBuffer->Release();
		s_Data.Shader->Release();
		s_Data.RenderTargets->Release();

		m_GraphicsAPI.Release();
	}
}


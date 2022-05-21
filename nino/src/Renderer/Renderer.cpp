#include "corepch.h"
#include "Renderer.h"

#include "Core/Window.h"

#include "Renderer/RenderTargets.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/ConstantBuffer.h"

namespace nino
{
	using namespace DirectX;

	struct RendererData
	{
		std::shared_ptr<RenderTargets> RenderTargets;
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<VertexBuffer> TriangleVertexBuffer;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<VertexBuffer> CubeVertexBuffer;
		std::shared_ptr<IndexBuffer> TriangleIndexBuffer;
		std::shared_ptr<IndexBuffer> QuadIndexBuffer;
		std::shared_ptr<IndexBuffer> CubeIndexBuffer;
		std::shared_ptr<ConstantBuffer> TriangleConstantBuffer;
		std::shared_ptr<ConstantBuffer> QuadConstantBuffer;
		std::shared_ptr<ConstantBuffer> CubeConstantBuffer;
	};

	static RendererData s_Data;
	bool Renderer::s_VSync = false;

	Renderer::Renderer(Window* window)
		: m_GraphicsAPI(window)
	{
		m_AspectRatio = (float)window->GetWidth() / (float)window->GetHeight();

		s_Data.RenderTargets = std::make_shared<RenderTargets>(window->GetWidth(), window->GetHeight(), &m_GraphicsAPI);
		s_Data.Shader = std::make_shared<Shader>(L"", &m_GraphicsAPI);

		s_Data.TriangleVertexBuffer = std::make_shared<VertexBuffer>(&m_GraphicsAPI);
		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(&m_GraphicsAPI);
		s_Data.CubeVertexBuffer = std::make_shared<VertexBuffer>(&m_GraphicsAPI);

		s_Data.TriangleIndexBuffer = std::make_shared<IndexBuffer>(&m_GraphicsAPI);
		s_Data.QuadIndexBuffer = std::make_shared<IndexBuffer>(&m_GraphicsAPI);
		s_Data.CubeIndexBuffer = std::make_shared<IndexBuffer>(&m_GraphicsAPI);

		s_Data.TriangleConstantBuffer = std::make_shared<ConstantBuffer>(&m_GraphicsAPI);
		s_Data.QuadConstantBuffer = std::make_shared<ConstantBuffer>(&m_GraphicsAPI);
		s_Data.CubeConstantBuffer = std::make_shared<ConstantBuffer>(&m_GraphicsAPI);

		s_Data.RenderTargets->SetViewports(window->GetWidth(), window->GetHeight());
		s_Data.Shader->SetLayout();

		s_Data.TriangleConstantBuffer->SetCamera({ 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 0.0f });
		s_Data.QuadConstantBuffer->SetCamera({ 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 0.0f });
		s_Data.CubeConstantBuffer->SetCamera({ 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 0.0f });

		s_Data.TriangleConstantBuffer->SetProjection(45.0f, m_AspectRatio);
		s_Data.QuadConstantBuffer->SetProjection(45.0f, m_AspectRatio);
		s_Data.CubeConstantBuffer->SetProjection(45.0f, m_AspectRatio);

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

	void Renderer::DrawTriangle(vec4f color, vec3f position, vec3f size, vec3f rotation)
	{
		mat4 transform = XMMatrixTranslation(position.x, position.y, position.z)
					   * XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))
					   * XMMatrixScaling(size.x, size.y, size.z);

		s_Data.TriangleVertexBuffer->SetVertexBuffer(color, PrimitiveType::TRIANGLE);
		s_Data.TriangleConstantBuffer->UploadBuffer(transform);

		DrawPrimitive(PrimitiveType::TRIANGLE);
	}

	void Renderer::DrawQuad(vec4f color, vec3f position, vec3f size, vec3f rotation)
	{
		mat4 transform = XMMatrixTranslation(position.x, position.y, position.z)
			* XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))
			* XMMatrixScaling(size.x, size.y, size.z);

		s_Data.QuadVertexBuffer->SetVertexBuffer(color, PrimitiveType::QUAD);
		s_Data.QuadConstantBuffer->UploadBuffer(transform);

		DrawPrimitive(PrimitiveType::QUAD);
	}

	void Renderer::DrawCube(vec4f color, vec3f position, vec3f size, vec3f rotation)
	{
		mat4 transform = XMMatrixTranslation(position.x, position.y, position.z)
			* XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))
			* XMMatrixScaling(size.x, size.y, size.z);

		s_Data.CubeVertexBuffer->SetVertexBuffer(color, PrimitiveType::CUBE);
		s_Data.CubeConstantBuffer->UploadBuffer(transform);

		DrawPrimitive(PrimitiveType::CUBE);
	}

	void Renderer::Draw()
	{
		s_Data.RenderTargets->Present(s_VSync);
	}

	void Renderer::DrawPrimitive(PrimitiveType primitive)
	{
		switch (primitive)
		{
		case PrimitiveType::TRIANGLE:
		{
			s_Data.TriangleIndexBuffer->SetIndexBuffer(primitive);
			s_Data.RenderTargets->Draw(3);

			break;
		}

		case PrimitiveType::QUAD:
		{
			s_Data.QuadIndexBuffer->SetIndexBuffer(primitive);
			s_Data.RenderTargets->Draw(6);

			break;
		}

		case PrimitiveType::CUBE:
		{
			s_Data.CubeIndexBuffer->SetIndexBuffer(primitive);
			s_Data.RenderTargets->Draw(36);

			break;
		}
		}
	}

	Renderer::~Renderer()
	{
		s_Data.TriangleConstantBuffer->Release();
		s_Data.QuadConstantBuffer->Release();
		s_Data.CubeConstantBuffer->Release();
		s_Data.TriangleIndexBuffer->Release();
		s_Data.QuadIndexBuffer->Release();
		s_Data.CubeIndexBuffer->Release();
		s_Data.TriangleVertexBuffer->Release();
		s_Data.QuadVertexBuffer->Release();
		s_Data.CubeVertexBuffer->Release();

		s_Data.Shader->Release();
		s_Data.RenderTargets->Release();

		m_GraphicsAPI.Release();
	}
}


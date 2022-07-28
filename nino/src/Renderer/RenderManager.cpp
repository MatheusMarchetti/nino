#include "corepch.h"
#include "RenderManager.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Primitives/Primitives.h"

namespace nino
{
	RenderManager::RenderManager()
	{
		GraphicsAPI::CreateDeviceAndContext();

		s_Instance = this;
	}

	void RenderManager::SetGraphicsConfiguration(GraphicsDescriptor& descriptor)
	{
		m_Descriptor = descriptor;
		s_VSync = m_Descriptor.vSync;
		m_AnisotropicFiltering = m_Descriptor.AnisotropyLevel;

		GraphicsAPI::SetTextureQuality(m_Descriptor.TextureQuality);
	}

	void RenderManager::SetRenderTarget(Window* window)
	{
		GraphicsAPI::CreateSwapChain(window);
	}

	void RenderManager::BeginScene(const Camera& camera)
	{
		auto& cameraCB = camera.GetShaderBuffer();
		
	}

	void RenderManager::EndScene(std::initializer_list<Ref<RenderPass>> renderPasses)
	{
		s_RenderPasses = { renderPasses };
		s_Instance->ComposeFrame();
	}

	void RenderManager::DrawPrimitive(const PrimitiveDescriptor& descriptor)
	{
		switch (descriptor.Type)
		{
		case PrimitiveType::Line: 
		{
			auto data = Primitives::DrawLine(descriptor.Position, descriptor.Rotation, descriptor.Scale.x, descriptor.Color);

			break;
		}
		case PrimitiveType::Triangle: 
		{
			auto data = Primitives::DrawTriangle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Quad: 
		{
			auto data = Primitives::DrawQuad(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Circle: 
		{
			auto data = Primitives::DrawCircle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cube: 
		{
			auto data = Primitives::DrawCube(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Sphere: 
		{
			auto data = Primitives::DrawSphere(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Capsule: 
		{
			auto data = Primitives::DrawCapsule(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cylinder: 
		{
			auto data = Primitives::DrawCylinder(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Torus: 
		{
			auto data = Primitives::DrawTorus(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Terrain: 
		{
			auto data = Primitives::DrawTerrain(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Mesh: 
		{
			auto data = Primitives::DrawMesh(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.MeshFile);
			break;
		}
		case PrimitiveType::Ragdoll:
		{
			auto data = Primitives::DrawRagdoll(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.MeshFile);
			break;
		}
		default:
			throw std::exception("Unknown primitive primitive");
		}
	}

	void RenderManager::DrawGrid(const float width, const float height)
	{

	}

	void RenderManager::EndFrame()
	{
		GraphicsAPI::Present(s_VSync);
	}

	void RenderManager::ComposeFrame()
	{

	}
}
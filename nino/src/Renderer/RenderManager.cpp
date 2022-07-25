#include "corepch.h"
#include "RenderManager.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Primitives/Primitives.h"

namespace nino
{
	RenderManager::RenderManager()
	{
		GraphicsAPI::CreateDeviceAndContext();
	}

	void RenderManager::SetGraphicsConfiguration(GraphicsDescriptor& descriptor)
	{
		m_Descriptor = descriptor;
		s_VSync = m_Descriptor.vSync;

		GraphicsAPI::SetTextureQuality(m_Descriptor.TextureQuality);
	}

	void RenderManager::SetRenderTarget(Window* window)
	{
		GraphicsAPI::CreateSwapChain(window);
	}

	void RenderManager::BeginScene()
	{
	}

	void RenderManager::EndScene(std::initializer_list<Ref<Framebuffer>> framebuffers)
	{
		GraphicsAPI::BindFramebuffers(framebuffers);
	}

	void RenderManager::DrawPrimitive(const PrimitiveDescriptor& descriptor)
	{
		switch (descriptor.Type)
		{
		case PrimitiveType::Line: 
		{
			Primitives::DrawLine(descriptor.Position, descriptor.Rotation, descriptor.Scale.x, descriptor.Color);
			break;
		}
		case PrimitiveType::Triangle: 
		{
			Primitives::DrawTriangle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Quad: 
		{
			Primitives::DrawQuad(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Circle: 
		{
			Primitives::DrawCircle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cube: 
		{
			Primitives::DrawCube(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Sphere: 
		{
			Primitives::DrawSphere(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Capsule: 
		{
			Primitives::DrawCapsule(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cylinder: 
		{
			Primitives::DrawCylinder(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Torus: 
		{
			Primitives::DrawTorus(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Terrain: 
		{
			Primitives::DrawTerrain(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Mesh: 
		{
			Primitives::DrawMesh(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.MeshFile);
			break;
		}
		case PrimitiveType::Ragdoll:
		{
			Primitives::DrawRagdoll(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.MeshFile);
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
}
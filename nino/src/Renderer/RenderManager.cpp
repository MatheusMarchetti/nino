#include "corepch.h"
#include "RenderManager.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	RenderManager::RenderManager()
	{
		s_VSync = false;
		GraphicsAPI::CreateDeviceAndContext();
	}

	void RenderManager::SetRenderTarget(Window* window)
	{
		GraphicsAPI::CreateSwapChain(window);
	}

	void RenderManager::BeginScene()
	{
	}

	void RenderManager::EndScene(const std::vector<Ref<Framebuffer>>& framebuffers)
	{
		GraphicsAPI::BindFramebuffers(framebuffers);
	}

	void RenderManager::DrawPrimitive(const PrimitiveDescriptor& descriptor)
	{
		switch (descriptor.Type)
		{
		case PrimitiveType::Line: 
		{
			DrawLine(descriptor.Position, descriptor.Rotation, descriptor.Scale[0], descriptor.Color);
			break;
		}
		case PrimitiveType::Triangle: 
		{
			DrawTriangle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Quad: 
		{
			DrawQuad(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Circle: 
		{
			DrawCircle(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cube: 
		{
			DrawCube(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Sphere: 
		{
			DrawSphere(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Capsule: 
		{
			DrawCapsule(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Cylinder: 
		{
			DrawCylinder(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Torus: 
		{
			DrawTorus(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Terrain: 
		{
			DrawTerrain(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.Color);
			break;
		}
		case PrimitiveType::Mesh: 
		{
			DrawMesh(descriptor.Position, descriptor.Rotation, descriptor.Scale, descriptor.MeshFile);
			break;
		}
		default:
			throw std::exception("Unknown primitive primitive");
		}
	}

	void RenderManager::EndScenes()
	{
		GraphicsAPI::Present(s_VSync);
	}

	void RenderManager::DrawLine(const float* Position, const float* Direction, float Size, const float* Color)
	{
	}

	void RenderManager::DrawTriangle(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}

	void RenderManager::DrawQuad(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}

	void RenderManager::DrawCircle(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}

	void RenderManager::DrawCube(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}

	void RenderManager::DrawSphere(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}
	void RenderManager::DrawCapsule(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}
	void RenderManager::DrawCylinder(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}
	void RenderManager::DrawTorus(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}
	void RenderManager::DrawTerrain(const float* Position, const float* Rotation, const float* Scale, const float* Color)
	{
	}
	void RenderManager::DrawMesh(const float* Position, const float* Rotation, const float* Scale, const char* MeshFile)
	{
	}
}
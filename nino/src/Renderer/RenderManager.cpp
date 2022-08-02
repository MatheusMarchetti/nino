#include "corepch.h"
#include "RenderManager.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"

namespace nino
{
	RenderManager::RenderManager()
	{
#ifdef NINO_API_D3D11
		m_GraphicsAPI = new D3D11Backend();
#else
		// Add default for d3d12
#endif

		s_Instance = this;
	}

	RenderManager::~RenderManager()
	{
		delete m_GraphicsAPI;
	}

	void RenderManager::SetGraphicsConfiguration(GraphicsDescriptor& descriptor)
	{
		m_Descriptor = descriptor;
		s_VSync = m_Descriptor.vSync;
		m_AnisotropicFiltering = m_Descriptor.AnisotropyLevel;

		m_GraphicsAPI->SetTextureQuality(m_Descriptor.TextureQuality);
	}

	void RenderManager::SetRenderTarget(Window* window)
	{
		s_Instance->m_GraphicsAPI->CreateSwapChain(window);
	}

	void RenderManager::BeginScene(const Camera& camera)
	{
		auto& cameraCB = camera.GetShaderBuffer();
	}

	void RenderManager::EndScene()
	{
		s_Instance->ComposeFrame();
	}

	void RenderManager::EndFrame()
	{
		s_Instance->m_GraphicsAPI->Present(s_VSync);
	}

	void RenderManager::ComposeFrame()
	{

	}
}
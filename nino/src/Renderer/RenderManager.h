#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Framebuffer.h"

namespace nino
{
	struct GraphicsDescriptor
	{
		uint32_t AnisotropyLevel;
		float TextureQuality;
		bool vSync;
	};

class Window;
class GraphicsAPI;

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void SetGraphicsConfiguration(GraphicsDescriptor& descriptor);

		static void SetRenderTarget(Window* window);
		static void ToggleVSync(bool vsync) { s_VSync = vsync; }

		static void BeginScene(const Camera& camera);
		static void EndScene();

		void EndFrame();

	private:
		void ComposeFrame();

	private:
		GraphicsAPI* m_GraphicsAPI;
		inline static RenderManager* s_Instance;
		GraphicsDescriptor m_Descriptor;
		uint32_t m_AnisotropicFiltering;
		inline static bool s_VSync;
	};
}

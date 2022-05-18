#pragma once

#include "Core.h"

namespace nino
{
	class Window;
	class GraphicsAPI;
	class CommandManager;
	class GraphicsContext;
	class BackBuffer;
}

namespace nino
{
	class CORE_API Renderer
	{
	public:
		Renderer(std::shared_ptr<Window> window);
		~Renderer() {}

		static void ToggleVSync(bool vSync) { s_VSync = vSync; }

		static void Clear(float color[4], float depth = 1.0f);
		static void Draw();
		
	private:
		static bool s_VSync;
		float m_AspectRatio;

		std::shared_ptr<GraphicsAPI> m_GraphicsAPI;
		std::shared_ptr<CommandManager> m_CommandManager;
		std::shared_ptr<GraphicsContext> m_GraphicsContext;
	};
}

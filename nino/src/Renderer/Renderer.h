#pragma once

#include "Core.h"

namespace nino
{
	class Window;
	class GraphicsAPI;
	class CommandManager;
	class GraphicsContext;
}

namespace nino
{
	class CORE_API Renderer
	{
	public:
		Renderer(Window* window);
		~Renderer() {}

		static void ToggleVSync(bool vSync) { s_VSync = vSync; }

		static void Clear(float color[4], float depth = 1.0f);
		static void Draw();
		
	private:
		static bool s_VSync;
		float m_AspectRatio;

		GraphicsAPI m_GraphicsAPI;
		CommandManager m_CommandManager;
		GraphicsContext m_GraphicsContext;
	};
}

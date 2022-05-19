#pragma once

#include "Core.h"

#include "Renderer/GraphicsAPI.h"
#include "Renderer/CommandManager.h"
#include "Renderer/GraphicsContext.h"

namespace nino
{
	class Window;
}

namespace nino
{
	class CORE_API Renderer
	{
	public:
		Renderer(Window* window);
		~Renderer();

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

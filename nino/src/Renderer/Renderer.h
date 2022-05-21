#pragma once

#include "Core.h"

#include "Renderer/GraphicsAPI.h"

#include "NinoMath.h"

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

		static void SetTargets();
		static void SetViewport(uint32_t width, uint32_t height);
		static void Clear(float color[4], float depth = 1.0f);

		// Draw Primitives
		static void DrawTriangle(vec4f color, vec3f position, vec3f size, vec3f rotation = {});
		static void DrawQuad(vec4f color, vec3f position, vec3f size, vec3f rotation = {});
		static void DrawCube(vec4f color, vec3f position, vec3f size, vec3f rotation = {});

		static void Draw();

	private:
		static void DrawPrimitive(PrimitiveType primitive);
		
	private:
		static bool s_VSync;
		float m_AspectRatio;

		GraphicsAPI m_GraphicsAPI;
	};
}

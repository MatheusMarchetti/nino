#pragma once

#include "Renderer/Framebuffer.h"

namespace nino
{

class Window;

	struct PrimitiveDescriptor
	{
		PrimitiveType Type;
		float* Position;
		float* Rotation;
		float* Scale;
		float* Color;
		const char* MeshFile = nullptr;
	};

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager() = default;

		static void SetRenderTarget(Window* window);

		static void BeginScene();
		static void EndScene(std::initializer_list<Ref<Framebuffer>> framebuffers);

		static void DrawPrimitive(const PrimitiveDescriptor& descriptor);

		void EndScenes();

	private:
		inline static bool s_VSync;
	};
}

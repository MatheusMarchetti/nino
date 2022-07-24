#pragma once

#include "Renderer/Framebuffer.h"

namespace nino
{

class Window;

	enum class PrimitiveType
	{
		Line,
		Triangle,
		Quad,
		Circle,
		Cube,
		Sphere,
		Capsule,
		Cylinder,
		Torus,
		Terrain,
		Mesh,
		Ragdoll
	};

	struct PrimitiveDescriptor
	{
		PrimitiveType Type;
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;
		Vector4 Color;
		const char* MeshFile = nullptr;
	};

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager() = default;

		static void SetRenderTarget(Window* window);
		static void ToggleVSync(bool vsync) { s_VSync = vsync; }

		static void BeginScene();
		static void EndScene(std::initializer_list<Ref<Framebuffer>> framebuffers);

		static void DrawPrimitive(const PrimitiveDescriptor& descriptor);
		static void DrawGrid(const float width, const float height);

		void EndFrame();

	private:
		inline static bool s_VSync;
	};
}

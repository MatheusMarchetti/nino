#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"

namespace nino
{
	struct GraphicsDescriptor
	{
		uint32_t AnisotropyLevel;
		float TextureQuality;
		bool vSync;
	};

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

		void SetGraphicsConfiguration(GraphicsDescriptor& descriptor);

		static void SetRenderTarget(Window* window);
		static void ToggleVSync(bool vsync) { s_VSync = vsync; }

		static void BeginScene(const Camera& camera);
		static void EndScene(std::initializer_list<Ref<RenderPass>> renderPasses);

		static void DrawPrimitive(const PrimitiveDescriptor& descriptor);
		static void DrawGrid(const float width, const float height);

		void EndFrame();

	private:
		void ComposeFrame();

	private:
		inline static RenderManager* s_Instance;
		GraphicsDescriptor m_Descriptor;
		uint32_t m_AnisotropicFiltering;
		inline static bool s_VSync;
		inline static std::vector<Ref<RenderPass>> s_RenderPasses;
	};
}

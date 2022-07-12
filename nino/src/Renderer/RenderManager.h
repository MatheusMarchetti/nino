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
		Mesh
	};

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

		//Render commands
		static void DrawPrimitive(const PrimitiveDescriptor& descriptor);

		void EndScenes();

	private:
		//2D primitives
		static void DrawLine(const float* Position, const float* Direction, float Size, const float* Color);
		static void DrawTriangle(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawQuad(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawCircle(const float* Position, const float* Rotation, const float* Scale, const float* Color);

		//3D primitives
		static void DrawCube(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawSphere(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawCapsule(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawCylinder(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawTorus(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawTerrain(const float* Position, const float* Rotation, const float* Scale, const float* Color);
		static void DrawMesh(const float* Position, const float* Rotation, const float* Scale, const char* MeshFile);

	private:
		inline static bool s_VSync;
	};
}

#pragma once

namespace nino
{
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

	class Primitives
	{
	public:
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
	};
}
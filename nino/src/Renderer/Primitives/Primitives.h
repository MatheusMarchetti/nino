#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Primitives
	{
		struct alignas(16) VertexData
		{
			Vector3 Position;
			Vector4 Color;
		};

	public:
		//2D primitives
		static void DrawLine(const Vector3& Position, const Vector3& Direction, float Size, const Vector4& Color);
		static void DrawTriangle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawQuad(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawCircle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);

		//3D primitives
		static void DrawCube(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawSphere(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawCapsule(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawCylinder(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawTorus(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawTerrain(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Vector4& Color);
		static void DrawMesh(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile);
		static void DrawRagdoll(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile);
	};
}
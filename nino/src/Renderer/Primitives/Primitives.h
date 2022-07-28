#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	struct alignas(16) VertexPosition
	{
		Vector3 Position;
	};

	struct alignas(16) Vertex2DData
	{
		Color Color;
		Vector3 Position;
		Vector2 TextureCoordinate;
	};

	struct alignas(16) Vertex3DData
	{
		Color Color;
		Vector3 Position;
		Vector3 Normal;
		Vector2 TextureCoordinate;
		Vector3 Tangent;
		Vector3 Bitangent;
	};

	class Primitives
	{
	public:
		//2D primitives
		static std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> DrawLine(const Vector3& Position, const Vector3& Direction, float Size, const Color& Color);
		static std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> DrawTriangle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> DrawQuad(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> DrawCircle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> DrawTerrain(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);

		//3D primitives
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawCube(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawSphere(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawCapsule(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawCylinder(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawTorus(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawMesh(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile);
		static std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> DrawRagdoll(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile);
	};
}
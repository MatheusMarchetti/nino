#include "corepch.h"
#include "Primitives.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> Primitives::DrawLine(const Vector3& Position, const Vector3& Direction, float Size, const Color& Color)
	{
		std::vector<Vertex2DData> vertexData(2);
		std::vector<unsigned short> indices(2);

		vertexData[0].Position = { -0.5f, 0.0f, 0.0f };
		vertexData[1].Position = { 0.5f, 0.0f, 0.0f };
		vertexData[0].Color = Color;
		vertexData[1].Color = Color;
		vertexData[0].TextureCoordinate = {};
		vertexData[1].TextureCoordinate = {};

		indices[0] = 0;
		indices[1] = 1;

		vertexData[0].Position = Matrix::CreateTranslation(Position) * Matrix::CreateFromYawPitchRoll(Direction) * vertexData[0].Position;
		vertexData[1].Position = Matrix::CreateTranslation(Position) * Matrix::CreateFromYawPitchRoll(Direction) * vertexData[1].Position;

		vertexData[0].Position.Normalize();
		vertexData[1].Position.Normalize();

		vertexData[0].Position = vertexData[0].Position * Size;
		vertexData[1].Position = vertexData[1].Position * Size;

		return { vertexData, indices };
	}

	std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> Primitives::DrawTriangle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}

	std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> Primitives::DrawQuad(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}

	std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> Primitives::DrawCircle(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}

	std::pair<std::vector<Vertex2DData>, std::vector<unsigned short>> Primitives::DrawTerrain(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}

	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawCube(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawSphere(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawCapsule(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawCylinder(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawTorus(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const Color& Color)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawMesh(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile)
	{
		return {};
	}
	std::pair<std::vector<Vertex3DData>, std::vector<unsigned short>> Primitives::DrawRagdoll(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale, const char* MeshFile)
	{
		return {};
	}
}
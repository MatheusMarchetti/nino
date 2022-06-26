#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		Camera();
		Camera(const DirectX::XMFLOAT4X4& view);

		void UpdateView(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation);
		void SetPerspective(float fov, float aspectRatio, float nearZ, float farZ);
		void SetOrthographic(float size, float nearZ, float farZ);
		void MakeIsometric(float height);

		const DirectX::XMFLOAT4X4& GetViewProjection() const { return m_ViewProjection; }
		DirectX::XMFLOAT3& GetFocalPoint() { return m_FocalPoint; }
		const ProjectionType GetProjectionType() const { return m_ProjectionType; }
		const float GetFieldOfView() const { return m_FOV; }
		const float GetAspectRatio() const { return m_AspectRatio; }
		const float GetNearPlane() const { return m_NearZ; }
		const float GetFarPlane() const { return m_FarZ; }
		const float GetViewSize() const { return m_Height; }

	private:
		void CalculateViewProjection();

	private:
		float m_FOV = 0.785f;
		float m_AspectRatio = 1280.0f / 720.0f;
		float m_NearZ = 0.1f;
		float m_FarZ = 1000.0f;
		float m_Height = 720.0f;
		float m_Width = 1280.0f;
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		DirectX::XMFLOAT3 m_FocalPoint;
		DirectX::XMFLOAT4X4 m_View;
		DirectX::XMFLOAT4X4 m_Projection;
		DirectX::XMFLOAT4X4 m_ViewProjection;
	};
}

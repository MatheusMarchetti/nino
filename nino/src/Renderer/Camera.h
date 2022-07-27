#pragma once

#include "Core/Core.h"

#include "Renderer/Buffer.h"

namespace nino
{
	struct CameraCBuf
	{
		Matrix ViewProjection;
	};

	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	class Camera
	{
	public:
		Camera();
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
		virtual ~Camera() = default;

		float GetDistance() { return m_Distance; }
		void SetDistanceFromTarget(float distance);
		void SetTarget(const Vector3& target);
		void UpdateCameraOrientation(const Vector3 orientation);

		void SetViewportSize(float width, float height);
		void SetFieldOfView(float fov);
		void ChangeCameraType(CameraType newType);
		void SetClippingPlanes(float nearPlane, float farPlane);

		Matrix& GetViewMatrix() { return m_View; }
		Matrix& GetProjectionMatrix() { return m_Projection; }

		Vector3& GetPosition() { return m_Position; }
		Quaternion& GetOrientation();

		const Ref<ConstantBuffer<CameraCBuf>>& GetShaderBuffer() const { return m_ConstantBuffer; }

	private:
		void UpdateProjection();
		void UpdateView();

		Vector3 CalculatePosition();

	private:
		CameraType m_Type;

		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_Width = 1280.0f;
		float m_Height = 720.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 1000.0f;

		float m_Distance = 10.0f;

		Vector3 m_Position = { 0.0f, 0.0f, 0.0f };
		Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 m_Target = { 0.0f, 0.0f, 0.0f };

		Matrix m_Projection;
		Matrix m_View;

		Ref<ConstantBuffer<CameraCBuf>> m_ConstantBuffer;
	};
}
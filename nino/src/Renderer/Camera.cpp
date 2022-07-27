#include "corepch.h"
#include "Camera.h"

namespace nino
{
	Camera::Camera()
	{
		m_Type = CameraType::Perspective;
		m_ConstantBuffer = CreateRef<ConstantBuffer<CameraCBuf>>();

		UpdateProjection();
		UpdateView();
	}

	Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
		m_Type = CameraType::Perspective;
		m_ConstantBuffer = CreateRef<ConstantBuffer<CameraCBuf>>();

		UpdateProjection();
		UpdateView();
	}

	void Camera::SetDistanceFromTarget(float distance)
	{
		m_Distance = distance;

		UpdateView();
	}

	void Camera::SetTarget(const Vector3& target)
	{
		m_Target = target;

		UpdateView();
	}

	void Camera::UpdateCameraOrientation(const Vector3 orientation)
	{
		m_Rotation = orientation;

		UpdateView();
	}

	void Camera::SetViewportSize(float width, float height)
	{
		m_Width = width;
		m_Height = height;
		m_AspectRatio = m_Width / m_Height;

		UpdateProjection();
	}

	void Camera::SetFieldOfView(float fov)
	{
		m_FOV = fov;

		UpdateProjection();
	}

	void Camera::ChangeCameraType(CameraType newType)
	{
		m_Type = newType;

		UpdateProjection();
	}

	void Camera::SetClippingPlanes(float nearPlane, float farPlane)
	{
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;

		UpdateProjection();
	}

	Quaternion& Camera::GetOrientation()
	{
		return Quaternion(Vector4(-m_Rotation.x, -m_Rotation.y, 0.0f, 0.0f));
	}

	void Camera::UpdateProjection()
	{
		switch (m_Type)
		{
		case nino::CameraType::Orthographic:
		{
			m_Projection = Matrix::CreatePerspectiveFieldOfView(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
			break;
		}
		case nino::CameraType::Perspective:
		{
			m_Projection = Matrix::CreateOrthographic(m_Width, m_Height, m_NearPlane, m_FarPlane);
			break;
		}
		default:
			break;
		}

		CameraCBuf VPmatrix = { m_View * m_Projection };
		m_ConstantBuffer->SetData(VPmatrix);
	}

	void Camera::UpdateView()
	{
		m_Position = CalculatePosition();

		Quaternion rotation = GetOrientation();

		m_View = Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(m_Position);
		m_View = m_View.Invert();

		CameraCBuf VPmatrix = { m_View * m_Projection };
		m_ConstantBuffer->SetData(VPmatrix);
	}

	Vector3 Camera::CalculatePosition()
	{
		Matrix Rotation = Matrix::CreateFromQuaternion(GetOrientation());

		return m_Target - Rotation.Forward() * m_Distance;
	}
}
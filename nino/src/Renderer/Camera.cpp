#include "corepch.h"
#include "Camera.h"

namespace nino
{
	Camera::Camera(const Vector3& translation, const Vector3& rotation, float width, float height, float nearZ, float farZ)
	{
		m_ProjectionMatrix = Matrix::CreateOrthographic(width, height, nearZ, farZ);

		UpdateCamera(translation, rotation);
	}

	Camera::Camera(const Vector3& translation, const Vector3& rotation, float fov, float aspectRatio, float nearZ, float farZ)
	{
		m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearZ, farZ);

		UpdateCamera(translation, rotation);
	}

	void Camera::UpdateCamera(const Vector3& translation, const Vector3& rotation)
	{
		Matrix Translation = Matrix::CreateTranslation(translation);
		Quaternion Rotation = Quaternion::CreateFromYawPitchRoll(rotation);
		Matrix RotationMatrix = Matrix::CreateFromQuaternion(Rotation);
		Matrix Transformation = RotationMatrix * Translation;

		m_ViewMatrix = Transformation.Invert();

		CameraCBuf cameraVP{ m_ViewMatrix * m_ProjectionMatrix };

		m_ConstantBuffer = CreateScope<ConstantBuffer<CameraCBuf>>(cameraVP);
	}
}
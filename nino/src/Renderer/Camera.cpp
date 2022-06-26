#include "corepch.h"
#include "Camera.h"

namespace nino
{
	using namespace DirectX;

	Camera::Camera()
	{
		DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
		auto Perspective = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);

		XMStoreFloat3(&m_FocalPoint, { 0.0f, 0.0f, 0.0f });
		XMStoreFloat4x4(&m_Projection, Perspective);
		XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	}

	Camera::Camera(const DirectX::XMFLOAT4X4& view)
		: m_View(view)
	{
		auto Perspective = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);

		XMStoreFloat4x4(&m_Projection, Perspective);
		XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	}

	void Camera::UpdateView(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation)
	{
		auto Translation = XMVectorSet(translation.x, translation.y, translation.z + 0.1f, 1.0f);
		auto Focus = XMVectorSet(m_FocalPoint.x, m_FocalPoint.y, std::abs(m_FocalPoint.z), 1.0f);
		auto Up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
//		auto Rotation = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
		auto View = XMMatrixLookAtLH(Translation, Focus, Up);

		XMStoreFloat4x4(&m_View, View);

		CalculateViewProjection();
	}

	void Camera::SetPerspective(float fov, float aspectRatio, float nearZ, float farZ)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearZ = nearZ;
		m_FarZ = farZ;

		auto Perspective = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

		XMStoreFloat4x4(&m_Projection, Perspective);

		CalculateViewProjection();
	}

	void Camera::SetOrthographic(float size, float nearZ, float farZ)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_Height = size;
		m_Width = size * m_AspectRatio;
		m_NearZ = nearZ;
		m_FarZ = farZ;

		auto Orthographic = XMMatrixOrthographicLH(m_Width, m_Height, nearZ, farZ);

		XMStoreFloat4x4(&m_Projection, Orthographic);

		CalculateViewProjection();
	}

	void Camera::MakeIsometric(float height)
	{

		CalculateViewProjection();
	}

	void Camera::CalculateViewProjection()
	{
		auto View = XMMatrixTranspose(XMLoadFloat4x4(&m_View));
		auto Projection = XMMatrixTranspose(XMLoadFloat4x4(&m_Projection));

		XMStoreFloat4x4(&m_ViewProjection, XMMatrixMultiply(Projection, View)); // As I cannot transpose the MVP matrix before passing to shader, I have to invert multiplication order.
	}
}


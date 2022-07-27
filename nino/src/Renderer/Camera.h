#pragma once

#include "Core/Core.h"

#include "Renderer/Buffer.h"

namespace nino
{
	class Camera
	{
		struct CameraCBuf
		{
			Matrix ViewProjection;
		};

	public:
		Camera(const Vector3& translation, const Vector3& rotation, float width, float height, float nearZ, float farZ);
		Camera(const Vector3& translation, const Vector3& rotation, float fov, float aspectRatio, float nearZ, float farZ);
		~Camera() = default;

		void UpdateCamera(const Vector3& translation, const Vector3& rotation);

	private:
		Scope<ConstantBuffer<CameraCBuf>> m_ConstantBuffer;
		Matrix m_ProjectionMatrix;
		Matrix m_ViewMatrix;
	};
}
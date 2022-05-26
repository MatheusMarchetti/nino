#pragma once

#include "Renderer/GraphicsCore.h"

#include "Core/NinoMath.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	struct ObjectConstantBuffer
	{
		mat4 MVP;
	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer(GraphicsAPI* graphicsAPI);
		~ConstantBuffer() {}

		void SetCamera(vec3f position, vec3f target, vec3f up = { 0.0f, 1.0f, 0.0f });
		void SetProjection(float fov, float aspectRatio, float nearPlane = 0.01f, float farPlane = 10000.0f);
		void UploadBuffer(mat4 transform = DirectX::XMMatrixIdentity());

		void Release();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		ObjectConstantBuffer m_ObjectCB;

		mat4 m_MVP;
		mat4 m_Model;
		mat4 m_View;
		mat4 m_Projection;
		vec4 m_CameraPosition;
		vec4 m_CameraTarget;
		vec4 m_CameraUp;

		GraphicsAPI* m_GraphicsAPI;
	};
}
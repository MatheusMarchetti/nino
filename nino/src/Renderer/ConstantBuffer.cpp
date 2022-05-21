#include "corepch.h"
#include "ConstantBuffer.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	using namespace DirectX;

	ConstantBuffer::ConstantBuffer(GraphicsAPI* graphicsAPI)
		: m_GraphicsAPI(graphicsAPI)
	{
		auto device = m_GraphicsAPI->GetDevice();

		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.ByteWidth = sizeof(ObjectConstantBuffer);
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA cbSubresource = {};
		cbSubresource.pSysMem = &m_ObjectCB;

		ThrowOnError(device->CreateBuffer(&cbDesc, &cbSubresource, &m_ConstantBuffer));
	}

	void ConstantBuffer::SetCamera(vec3f position, vec3f target, vec3f up)
	{
		m_CameraPosition = XMVectorSet(position.x, position.y, position.z, 0.0f);
		m_CameraTarget = XMVectorSet(target.x, target.y, target.z, 0.0f);
		m_CameraUp = XMVectorSet(up.x, up.y, up.z, 0.0f);

		m_View = XMMatrixLookAtLH(m_CameraPosition, m_CameraTarget, m_CameraUp);
	}

	void ConstantBuffer::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspectRatio, nearPlane, farPlane);
	}

	void ConstantBuffer::UploadBuffer(mat4 transform)
	{
		auto context = m_GraphicsAPI->GetContext();

		m_Model = transform;

		m_MVP = m_Model * m_View * m_Projection;

		m_ObjectCB.MVP = XMMatrixTranspose(m_MVP);

		D3D11_MAPPED_SUBRESOURCE subResource;

		context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		memcpy(subResource.pData, &m_ObjectCB, sizeof(ObjectConstantBuffer));
		context->Unmap(m_ConstantBuffer.Get(), 0);

		context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
	}

	void ConstantBuffer::Release()
	{
		m_ConstantBuffer = nullptr;

		m_GraphicsAPI = nullptr;
	}
}


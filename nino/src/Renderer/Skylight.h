#pragma once

#include "Renderer/Bindable/BindableCore.h"
#include "Renderer/Drawable/IndexedTriangleList.h"

namespace nino
{
	class Skylight
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Position;
		};

	public:
		Skylight(const std::string& filepath);

		void SetData(const ModelCBuf data);

		template<class T>
		static IndexedTriangleList<T> Make(int latitude, int longitude)
		{
			latitude = latitude < 3 ? 3 : latitude;
			longitude = longitude < 3 ? 3 : longitude;

			constexpr float radius = 0.5f;
			constexpr float pi = 3.141592f;
			size_t numSphereVertices = (latitude - 2) * longitude + 2;
			size_t numSphereFaces = ((latitude - 3) * longitude * 2) + 2 * longitude;

			float sphereYaw = 0.0f;
			float spherePitch = 0.0f;

			std::vector<T> vertices(numSphereVertices);

			DirectX::XMVECTOR currentVertexPosition = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			vertices[0].Position = { 0.0f, 0.0f, radius };

			for (size_t i = 0; i < latitude - 2; i++)
			{
				spherePitch = (i + 1) * (pi / (latitude - 1));
				auto rotationX = DirectX::XMMatrixRotationX(spherePitch);

				for (size_t j = 0; j < longitude; j++)
				{
					sphereYaw = j * 2 * (pi / longitude);
					auto rotationY = DirectX::XMMatrixRotationZ(sphereYaw);

					currentVertexPosition = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationX * rotationY);
					currentVertexPosition = DirectX::XMVector3Normalize(currentVertexPosition);

					vertices[i * longitude + j + 1].Position = { DirectX::XMVectorGetX(currentVertexPosition), DirectX::XMVectorGetY(currentVertexPosition), DirectX::XMVectorGetZ(currentVertexPosition) };
				}
			}

			vertices[numSphereVertices - 1].Position = { 0.0f, 0.0f, -radius };

			std::vector<unsigned short> indices(numSphereFaces * 3);
			int k = 0;

			for (size_t l = 0; l < longitude - 1; l++)
			{
				indices[k] = 0;
				indices[k + 1] = l + 1;
				indices[k + 2] = l + 2;

				k += 3;
			}

			indices[k] = 0;
			indices[k + 1] = longitude;
			indices[k + 2] = 1;

			k += 3;

			for (size_t i = 0; i < latitude - 3; i++)
			{
				for (size_t j = 0; j < longitude - 1; j++)
				{
					indices[k] = i * longitude + j + 1;
					indices[k + 1] = i * longitude + j + 2;
					indices[k + 2] = (i + 1) * longitude + j + 1;

					indices[k + 3] = (i + 1) * longitude + j + 1;
					indices[k + 4] = i * longitude + j + 2;
					indices[k + 5] = (i + 1) * longitude + j + 2;

					k += 6;
				}

				indices[k] = i * longitude + longitude;
				indices[k + 1] = i * longitude + 1;
				indices[k + 2] = (i + 1) * longitude + longitude;

				indices[k + 3] = (i + 1) * longitude + longitude;
				indices[k + 4] = i * longitude + 1;
				indices[k + 5] = (i + 1) * longitude + 1;

				k += 6;
			}

			for (size_t l = 0; l < longitude - 1; l++)
			{
				indices[k] = numSphereVertices - 1;
				indices[k + 1] = (numSphereVertices - 1) - (l + 1);
				indices[k + 2] = (numSphereVertices - 1) - (l + 2);

				k += 3;
			}

			indices[k] = numSphereVertices - 1;
			indices[k + 1] = (numSphereVertices - 1) - longitude;
			indices[k + 2] = (numSphereVertices - 1) - 2;

			return { std::move(vertices), std::move(indices) };
		}

	private:
		IndexedTriangleList<Vertex> m_Model = Skylight::Make<Vertex>(12, 24);
		ModelConstantBuffer m_Transform;
	};
}
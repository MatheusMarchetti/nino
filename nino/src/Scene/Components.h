#pragma once

#include "Core/Core.h"

#include <DirectXMath.h>

#include <string>

namespace nino
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		DirectX::XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const DirectX::XMFLOAT3& translation)
			: Translation(translation) {}

		DirectX::XMMATRIX GetTransform() const
		{
			DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);

			return DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) 
				* DirectX::XMMatrixRotationQuaternion(quaternion) 
				* DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
		}
	};
}

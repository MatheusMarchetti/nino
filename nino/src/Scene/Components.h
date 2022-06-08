#pragma once

#include "Core/Core.h"

#include "Renderer/Drawable/DrawableBase.h"

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

		const DirectX::XMFLOAT4X4 GetTransform() const
		{
			DirectX::XMFLOAT4X4 transform;
			DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z)
				* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z))
				* DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z));

			return transform;
		}
	};

	struct DrawableComponent
	{
		Ref<Drawable> Model;

		DrawableComponent() = default;
		DrawableComponent(const DrawableComponent&) = default;
		DrawableComponent(Ref<Drawable> model)
			: Model(std::move(model)) {}
	};
}

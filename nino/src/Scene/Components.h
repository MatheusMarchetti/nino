#pragma once

#include "Core/Core.h"

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
		Vector3 Translation = { 0.0f, 0.0f, 0.0f };
		Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const float& x, const float& y, const float& z)
			: Translation(x, y, z) {}

		const Matrix GetTransform() const { return Matrix::CreateScale(Scale) * Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(Rotation)) * Matrix::CreateTranslation(Translation); }
	};
}

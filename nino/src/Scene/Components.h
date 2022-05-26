#pragma once

#include "Core/NinoMath.h"

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
		vec3f Translation = { 0.0f, 0.0f, 0.0f };
		vec3f Rotation = { 0.0f, 0.0f, 0.0f };
		vec3f Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const vec3f& translation) 
			: Translation(translation) {}

		mat4 GetTransform() const
		{
			vec4 quaternion = DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);

			return DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) 
				* DirectX::XMMatrixRotationQuaternion(quaternion) 
				* DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
		}
	};

	struct MeshComponent
	{
		vec4f Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const vec4f& color)
			: Color(color) {}
	};
}

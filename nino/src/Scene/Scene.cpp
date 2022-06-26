#include "corepch.h"
#include "Scene.h"

#include "Renderer/Renderer.h"

#include "Scene/Entity.h"
#include "Scene/Components.h"

namespace nino
{
	Entity Scene::CreateEntity(const std::string& entityTag)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>(entityTag);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::UpdateScene(Timestep ts)
	{
		auto cameraTransform = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto& entity : cameraTransform)
		{
			auto [camera, transform] = cameraTransform.get<CameraComponent, TransformComponent>(entity);

			if (camera.MainCamera)
			{
				camera.camera.UpdateView(transform.Translation, transform.Rotation);

				Renderer::BeginScene(camera.camera);
			}
		}

		auto transformDrawable = m_Registry.group<TransformComponent>(entt::get<DrawableComponent>);
		for (auto& entity : transformDrawable)
		{
			ModelCBuf modelTransform;

			auto [transform, drawable] = transformDrawable.get<TransformComponent, DrawableComponent>(entity);

			auto Model = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&transform.GetTransform()));
			DirectX::XMStoreFloat4x4(&modelTransform.ModelMatrix, Model);

			if (drawable.Model)
			{
				drawable.Model->SetData(modelTransform);
				drawable.Model->Draw();
			}
		}
	}
}


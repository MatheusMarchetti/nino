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

	void Scene::CreateSky(const std::string& filepath)
	{
		m_Skylight = CreateRef<Skylight>(filepath);
	}

	void Scene::UpdateScene(Timestep ts)
	{
		auto cameraTransform = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto& entity : cameraTransform)
		{
			auto [camera, transform] = cameraTransform.get<CameraComponent, TransformComponent>(entity);

			ModelCBuf cameraTransform;

			if (camera.MainCamera)
			{
				camera.camera.UpdateView(transform.Translation, transform.Rotation);
				transform.Scale = { 1000.0f, 1000.0f, 1000.0f };
				cameraTransform.ModelMatrix = transform.GetTransform();
				
				m_Skylight->SetData(cameraTransform);
				
				Renderer::BeginScene(camera.camera, *m_Skylight);
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


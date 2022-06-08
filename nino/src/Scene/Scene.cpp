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
		Renderer::BeginScene();

		auto group = m_Registry.group<TransformComponent>(entt::get<DrawableComponent>);
		for (auto& entity : group)
		{
			ModelCBuf modelTransform;

			auto [transform, drawable] = group.get<TransformComponent, DrawableComponent>(entity);

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


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
		// Draw here
	}
}


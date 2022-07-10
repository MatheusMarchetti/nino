#pragma once

#include "Core/Timestep.h"
#include "Core/Core.h"
#include "Renderer/Skylight.h"

#include "entt.hpp"

namespace nino
{
	class Entity;
	class GUILayer;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& entityTag =  { "Unnamed entity" });
		void DestroyEntity(Entity entity);

		void CreateSky(const std::string& filepath);

		void UpdateScene(Timestep ts);

	private:
		entt::registry m_Registry;
		Ref<Skylight> m_Skylight;

		friend class Entity;
		friend class GUILayer; // Change when there's an editor
	};
}
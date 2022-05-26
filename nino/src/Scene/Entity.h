#pragma once

#include "Scene/Scene.h"

#include "entt.hpp"

namespace nino
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene) : m_Entity(entity), m_Scene(scene) {}

		template<typename T, typename...Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);

			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_Entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_Entity);
		}

		operator bool() const { return m_Entity != entt::null; }
		operator entt::entity() const { return m_Entity; }
		operator uint32_t() const { return (uint32_t)m_Entity; }
		bool operator == (const Entity& other) const
		{
			return m_Entity == other.m_Entity && m_Scene == other.m_Scene;
		}
		bool operator != (const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_Entity{ entt::null };

		Scene* m_Scene = nullptr;
	};
}

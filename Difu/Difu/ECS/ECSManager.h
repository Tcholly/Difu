#pragma once

#include "Difu/Utils/Logger.h"

#include <raylib.h>
#include <entt.hpp>
#include <string>

namespace ECS
{
	class Entity
	{
	private:
		entt::entity entity_handle;
		entt::registry* registry_ref;

	public:
		Entity() = default;
		Entity(entt::entity, entt::registry* registry);

		// Check if entity is alive
		bool IsAlive();
		// Destroy the entity
		void Destroy();

		// Check if entity has component
		template<typename T>
		bool HasComponent()
		{
			return registry_ref->any_of<T>(entity_handle);
		}

		// Add component to entity, if entity already owns that component it will retrieve said component
		template<typename T>
		T& AddComponent()
		{
			if (HasComponent<T>())
				return registry_ref->get<T>(entity_handle);

			return registry_ref->emplace<T>(entity_handle);
		}

		// Removes component from entity
		template<typename T>
		void RemoveComponent()
		{
			if (HasComponent<T>())
				registry_ref->erase<T>(entity_handle);
		}

		// Get component from entity, if entity does not have the component it ??????
		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>())
			{
				// TODO: decide what to do
				LOG_WARN("Entity does not have component for GetComponent");
			}
			return registry_ref->get<T>(entity_handle);
		}
	};

	entt::registry& GetRegistry();

	void Update(float dt);
	void Render();

	Camera2D GetPrimaryCamera();

	Entity CreateEntity(const std::string& tag = std::string());
	Entity FindEntityByTag(const std::string& tag);
} // namespace ECS

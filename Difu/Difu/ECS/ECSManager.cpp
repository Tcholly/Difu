#include "ECSManager.h"

#include "Components.h"

#include <raylib.h>

namespace ECS
{
	static entt::registry registry;

	Entity::Entity(entt::entity entity, entt::registry* _registry) 
		: entity_handle(entity), registry_ref(_registry)
	{

	}

	bool Entity::IsAlive()
	{
		return registry_ref->valid(entity_handle);
	}

	void Entity::Destroy()
	{
		registry_ref->destroy(entity_handle);
	}


	entt::registry& GetRegistry()
	{
		return registry;
	}

	void Update(float dt)
	{
		(void)dt;
	}

	void Render()
	{
		// Draw textures 
		auto texture_renderer_view = registry.view<Transform2DComponent, TextureRendererComponent>();
		for (auto entity : texture_renderer_view)
		{
			Transform2DComponent& transform = texture_renderer_view.get<Transform2DComponent>(entity);
			TextureRendererComponent& renderer = texture_renderer_view.get<TextureRendererComponent>(entity);

			Rectangle dest_rect = {transform.position.x, transform.position.y, (float)renderer.texture.width * transform.scale.x, (float)renderer.texture.height * transform.scale.y};
			DrawTexturePro(renderer.texture, renderer.source_rectangle, dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, WHITE);
		}

		// Draw rectangles
		auto rect_renderer_view = registry.view<Transform2DComponent, RectangleRendererComponent>();
		for (auto entity : rect_renderer_view)
		{
			Transform2DComponent& transform = rect_renderer_view.get<Transform2DComponent>(entity);
			RectangleRendererComponent& renderer = rect_renderer_view.get<RectangleRendererComponent>(entity);

			Rectangle dest_rect = {transform.position.x, transform.position.y, renderer.size.x * transform.scale.x, renderer.size.y * transform.scale.y};
			DrawRectanglePro(dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, renderer.color);
		}

		// Draw circles
		auto circle_renderer_view = registry.view<Transform2DComponent, CircleRendererComponent>();
		for (auto entity : circle_renderer_view)
		{
			Transform2DComponent& transform = circle_renderer_view.get<Transform2DComponent>(entity);
			CircleRendererComponent& renderer = circle_renderer_view.get<CircleRendererComponent>(entity);

			DrawEllipse((int)transform.position.x, (int)transform.position.y, renderer.radius * transform.scale.x, renderer.radius * transform.scale.y, renderer.color);
		}
	}

	Entity CreateEntity(const std::string& tag)
	{
		entt::entity entity = registry.create();

		std::string final_tag = tag;
		if (final_tag.empty()) 
			final_tag = "Entity";

		registry.emplace<TagComponent>(entity, final_tag);

		return {entity, &registry};
	}
} // namespace ECS

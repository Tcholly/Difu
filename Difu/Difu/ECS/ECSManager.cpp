#include "ECSManager.h"

#include "Components.h"

#include <map>
#include <raylib.h>

namespace ECS
{
	static entt::registry registry;

	namespace Renderer
	{
		static void RenderTexture(entt::entity entity)
		{
				Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
				TextureRendererComponent& renderer = registry.get<TextureRendererComponent>(entity);
	
				Rectangle dest_rect = {transform.position.x, transform.position.y, (float)renderer.texture.width * transform.scale.x, (float)renderer.texture.height * transform.scale.y};
				DrawTexturePro(renderer.texture, renderer.source_rectangle, dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, WHITE);
		}

		static void RenderRectangle(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			RectangleRendererComponent& renderer = registry.get<RectangleRendererComponent>(entity);

			Rectangle dest_rect = {transform.position.x, transform.position.y, renderer.size.x * transform.scale.x, renderer.size.y * transform.scale.y};
			DrawRectanglePro(dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, renderer.color);
		}

		static void RenderCircle(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			CircleRendererComponent& renderer = registry.get<CircleRendererComponent>(entity);

			DrawEllipse((int)transform.position.x, (int)transform.position.y, renderer.radius * transform.scale.x, renderer.radius * transform.scale.y, renderer.color);
		}
	} // namespace Renderer

	// Entity -----------------------------------------------------
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

	// ------------------------------------------------------------


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
		{ // No Layer
			// Draw textures 
			auto texture_renderer_view = registry.view<Transform2DComponent, TextureRendererComponent>(entt::exclude<RenderLayerComponent>);
			for (auto entity : texture_renderer_view)
				Renderer::RenderTexture(entity);

			// Draw rectangles
			auto rect_renderer_view = registry.view<Transform2DComponent, RectangleRendererComponent>(entt::exclude<RenderLayerComponent>);
			for (auto entity : rect_renderer_view)
				Renderer::RenderRectangle(entity);

			// Draw circles
			auto circle_renderer_view = registry.view<Transform2DComponent, CircleRendererComponent>(entt::exclude<RenderLayerComponent>);
			for (auto entity : circle_renderer_view)
				Renderer::RenderCircle(entity);
		}

		auto render_layer_view = registry.view<RenderLayerComponent>();
		std::map<char, bool> active_layers;
		for (auto entity : render_layer_view)
		{
			const RenderLayerComponent& layer_component = render_layer_view.get<RenderLayerComponent>(entity);
			active_layers[layer_component.layer] = true;	
		}

		{ // Layers
		  	for (const auto& [key, value] : active_layers)
			{
				// Draw textures 
				auto texture_renderer_view = registry.view<Transform2DComponent, TextureRendererComponent, RenderLayerComponent>();
				for (auto entity : texture_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == key)
						Renderer::RenderTexture(entity);
				}

				// Draw rectangles
				auto rect_renderer_view = registry.view<Transform2DComponent, RectangleRendererComponent, RenderLayerComponent>();
				for (auto entity : rect_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == key)
						Renderer::RenderRectangle(entity);
				}

				// Draw circles
				auto circle_renderer_view = registry.view<Transform2DComponent, CircleRendererComponent, RenderLayerComponent>();
				for (auto entity : circle_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == key)
						Renderer::RenderCircle(entity);
				}
			}
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

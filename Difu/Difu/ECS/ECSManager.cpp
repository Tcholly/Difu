#include "ECSManager.h"

#include "Components.h"
#include "Difu/Utils/Logger.h"
#include "Difu/Utils/MathExtension.h"

#include <map>
#include <raylib.h>
#include <raymath.h>

namespace ECS
{
	static entt::registry registry;

	namespace Updater
	{
		static void UpdateParticleEmitter(entt::entity entity, float dt)
		{
			ParticleEmitterComponent& emitter = registry.get<ParticleEmitterComponent>(entity);

    		for (int i = emitter.particles.size() - 1; i > -1; i--)
    		{
    		    emitter.particles[i].lifetime -= dt;
    		    if (emitter.particles[i].lifetime < 0.0f)
    		    {
    		        emitter.particles.erase(emitter.particles.begin() + i);
    		        continue;
    		    }

				// Update every particle data
    		    emitter.particles[i].velocity += emitter.particleAcceleration * dt;
    		    emitter.particles[i].position += emitter.particles[i].velocity * dt;

    		    emitter.particles[i].rotationVelocity += emitter.particleRotationAcceleration * dt;
    		    emitter.particles[i].rotation += emitter.particles[i].rotationVelocity * dt;

				Vector2 perpendicularVec = { emitter.particles[i].velocity.y, -emitter.particles[i].velocity.x };
				perpendicularVec = Vector2Normalize(perpendicularVec);

				emitter.particles[i].velocity += perpendicularVec * emitter.centripetalAcceleration * dt;
    		}

    
    		if (emitter.emitting)
    		{
				// Count how many particles should be emitted
    		    emitter.spawnTimer += dt;
    		    int emitCount = 0;
    		    while (emitter.spawnTimer > emitter.spawnInterval)
    		    {
    		        emitCount++;
    		        emitter.spawnTimer -= emitter.spawnInterval;
    		    }

    		    if (emitCount > 0)
    		        emitter.EmitNow(emitCount);
    		}	
		}
	}

	namespace Renderer
	{
		static void RenderTexture(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			TextureRendererComponent& renderer = registry.get<TextureRendererComponent>(entity);
	
			Rectangle dest_rect;
			dest_rect.x = transform.position.x;
			dest_rect.y = transform.position.y;
			dest_rect.width = (float)renderer.texture.width * transform.scale.x;
			dest_rect.height = (float)renderer.texture.height * transform.scale.y;

			DrawTexturePro(renderer.texture, renderer.source_rectangle, dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, WHITE);
		}

		static void RenderRectangle(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			RectangleRendererComponent& renderer = registry.get<RectangleRendererComponent>(entity);

			Rectangle dest_rect;
			dest_rect.x = transform.position.x;
			dest_rect.y = transform.position.y;
			dest_rect.width = renderer.size.x * transform.scale.x;
			dest_rect.height = renderer.size.y * transform.scale.y;
			DrawRectanglePro(dest_rect, {dest_rect.width / 2.0f, dest_rect.height / 2.0f}, RAD2DEG * transform.rotation, renderer.color);
		}

		static void RenderCircle(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			CircleRendererComponent& renderer = registry.get<CircleRendererComponent>(entity);

			DrawEllipse((int)transform.position.x, (int)transform.position.y, renderer.radius * transform.scale.x, renderer.radius * transform.scale.y, renderer.color);
		}

		static void RenderParticleEmitter(entt::entity entity)
		{
			Transform2DComponent& transform = registry.get<Transform2DComponent>(entity);
			ParticleEmitterComponent& emitter = registry.get<ParticleEmitterComponent>(entity);

    		for (Particle particle : emitter.particles)
    		{
				// TODO: Rotate particles to match emitter transfor rotation
    		    float lifeProgress = 1.0f - particle.lifetime / emitter.lifetime;
    		    Color color = LerpColor(particle.startColor, particle.endColor, lifeProgress); 
    		    Rectangle rect;
				rect.x = particle.position.x + transform.position.x;
				rect.y = particle.position.y + transform.position.y;
				rect.width = particle.size.x * transform.scale.x;
				rect.height = particle.size.y * transform.scale.y;
    		    DrawRectanglePro(rect, {rect.width / 2.0f, rect.height / 2.0f}, RAD2DEG * particle.rotation, color);
    		}
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
		auto particle_emitter_view = registry.view<ParticleEmitterComponent>();
		for (auto entity : particle_emitter_view)
			Updater::UpdateParticleEmitter(entity, dt);
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

			// Draw particles
			auto particle_emitter_view = registry.view<Transform2DComponent, ParticleEmitterComponent>(entt::exclude<RenderLayerComponent>);
			for (auto entity : particle_emitter_view)
				Renderer::RenderParticleEmitter(entity);
		}

		auto render_layer_view = registry.view<RenderLayerComponent>();
		std::map<char, bool> active_layers;
		for (auto entity : render_layer_view)
		{
			const RenderLayerComponent& layer_component = render_layer_view.get<RenderLayerComponent>(entity);
			active_layers[layer_component.layer] = true;	
		}

		{ // Layers
		  	for (const auto& [layer, value] : active_layers)
			{
				// Draw textures 
				auto texture_renderer_view = registry.view<Transform2DComponent, TextureRendererComponent, RenderLayerComponent>();
				for (auto entity : texture_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == layer)
						Renderer::RenderTexture(entity);
				}

				// Draw rectangles
				auto rect_renderer_view = registry.view<Transform2DComponent, RectangleRendererComponent, RenderLayerComponent>();
				for (auto entity : rect_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == layer)
						Renderer::RenderRectangle(entity);
				}

				// Draw circles
				auto circle_renderer_view = registry.view<Transform2DComponent, CircleRendererComponent, RenderLayerComponent>();
				for (auto entity : circle_renderer_view)
				{
					const RenderLayerComponent& layer_component = texture_renderer_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == layer)
						Renderer::RenderCircle(entity);
				}

				// Draw particles
				auto particle_emitter_view = registry.view<Transform2DComponent, ParticleEmitterComponent, RenderLayerComponent>();
				for (auto entity : particle_emitter_view)
				{
					const RenderLayerComponent& layer_component = particle_emitter_view.get<RenderLayerComponent>(entity);
					if (layer_component.layer == layer)
						Renderer::RenderParticleEmitter(entity);
				}
			}
		}
	}
	
	Entity FindEntityByTag(const std::string &tag)
	{
		auto tag_view = registry.view<TagComponent>();
		for (auto entity : tag_view)
		{
			TagComponent& tag_component = tag_view.get<TagComponent>(entity);
			if (tag_component.tag == tag)
				return {entity, &registry};
		}

		LOG_WARN("Couldn't find entity with tag \"{}\"", tag);
		return {entt::null, &registry};
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

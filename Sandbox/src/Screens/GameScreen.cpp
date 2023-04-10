#include "GameScreen.h"
#include "Difu/ECS/Components.h"

#include <cmath>
#include <raylib.h>
#include <Difu/ECS/ECSManager.h>
#include <Difu/Utils/Logger.h>

namespace GameScreen
{
	static ECS::Entity player;

	static float time;

	void UpdatePlayer(ECS::Entity entity, float dt)
	{
		Transform2DComponent& player_transform = player.GetComponent<Transform2DComponent>();

		player_transform.rotation += 5 * dt;
		player_transform.scale.y = std::sin(time * 10) + 1;
	}
	

	void Load()
	{
		SetTargetFPS(60);
		
		player = ECS::CreateEntity("Player");
		ECS::Entity random_ent = ECS::CreateEntity();
		ECS::Entity particle_ent = ECS::CreateEntity("Emitter");

		auto& player_transform = player.AddComponent<Transform2DComponent>();
		player_transform.position = {200.0f, 200.0f};
		// player_transform.scale = {1.0f, 1.0f};

		auto& rect_component = player.AddComponent<RectangleRendererComponent>();
		rect_component.color = RED;
		rect_component.size = {50.0f, 50.0f};

		auto& layer_component = player.AddComponent<RenderLayerComponent>();
		layer_component.layer = 2;

		auto& player_script = player.AddComponent<ScriptComponent>();
		player_script.OnUpdate = &UpdatePlayer;

		auto& random_transform = random_ent.AddComponent<Transform2DComponent>();
		random_transform.position = {200.0f, 200.0f};
		// random_transform.scale = {1.0f, 1.0f};

		auto& circle_component = random_ent.AddComponent<CircleRendererComponent>();
		circle_component.color = GREEN;
		circle_component.radius = 20.0f;

		auto& random_layer_component = random_ent.AddComponent<RenderLayerComponent>();
		random_layer_component.layer = 1;

		auto& emitter_transform = particle_ent.AddComponent<Transform2DComponent>();
		emitter_transform.position = {200.0f, 200.0f};

		auto& emitter_component = particle_ent.AddComponent<ParticleEmitterComponent>();
		emitter_component.spawnVelocity = {0.0f, 100.0f};
		emitter_component.particleAcceleration = {1.0f, -30.0f};
		emitter_component.startColor = BLACK;
		emitter_component.endColor = WHITE;
		emitter_component.particleMaxSizeFactor = 10;
		emitter_component.lifetime = 5.0f;
		emitter_component.spawnInterval = 0.1f;
		emitter_component.spread = 3.14159265;
		emitter_component.StartEmitting();

		auto& emitter_layer_component = particle_ent.AddComponent<RenderLayerComponent>();
		emitter_layer_component.layer = 3;
	}

	void Unload()
	{
	}


	void Update(float dt)
	{
		time += dt;

		ECS::Entity particle_ent = ECS::FindEntityByTag("Emitter");
		Transform2DComponent& emitter_transform = particle_ent.GetComponent<Transform2DComponent>();
		emitter_transform.scale.x = (std::sin(time * 5) + 2) * 10;
	}

	void Render()
	{
		ClearBackground(WHITE);
	
		DrawText("HELLO WORLD?", 100, 100, 30, GRAY);
	}

	void OnResize(int width, int height)
	{
		(void)width;
		(void)height;
	}

    Screen GetScreen()
    {
        Screen screen;
        screen.LoadFunction = &Load;
        screen.UnloadFunction = &Unload;
        screen.UpdateFunction = &Update;
        screen.RenderFunction = &Render;
		screen.OnResize = &OnResize;

        return screen;
    }
}

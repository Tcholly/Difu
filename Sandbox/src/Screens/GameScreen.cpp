#include "GameScreen.h"
#include "Difu/ECS/Components.h"

#include <cmath>
#include <raylib.h>
#include <Difu/ECS/ECSManager.h>
#include <Difu/Utils/Logger.h>

namespace GameScreen
{
	static ECS::Entity player;

	void Load()
	{
		SetTargetFPS(60);
		
		player = ECS::CreateEntity("Player");
		ECS::Entity random_ent = ECS::CreateEntity();

		auto& player_transform = player.AddComponent<Transform2DComponent>();
		player_transform.position = {200.0f, 200.0f};
		// player_transform.scale = {1.0f, 1.0f};

		auto& rect_component = player.AddComponent<RectangleRendererComponent>();
		rect_component.color = RED;
		rect_component.size = {50.0f, 50.0f};

		auto& layer_component = player.AddComponent<RenderLayerComponent>();
		layer_component.layer = 2;

		auto& random_transform = random_ent.AddComponent<Transform2DComponent>();
		random_transform.position = {200.0f, 200.0f};
		// random_transform.scale = {1.0f, 1.0f};

		auto& circle_component = random_ent.AddComponent<CircleRendererComponent>();
		circle_component.color = GREEN;
		circle_component.radius = 20.0f;

		auto& random_layer_component = random_ent.AddComponent<RenderLayerComponent>();
		random_layer_component.layer = 1;
	}

	void Unload()
	{
	}

	static float time;

	void Update(float dt)
	{
		time += dt;
		Transform2DComponent& player_transform = player.GetComponent<Transform2DComponent>();

		player_transform.rotation += 5 * dt;
		player_transform.scale.y = std::sin(time * 10) + 1;
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

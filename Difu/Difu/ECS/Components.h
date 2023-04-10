#pragma once

#include <string>
#include <raylib.h>

#include "Difu/Particles/ParticleEmitter.h"

namespace ECS
{
	class Entity;
}

struct TagComponent
{
	std::string tag;
};

struct Transform2DComponent
{
	Vector2 position;
	float rotation;
	Vector2 scale = {1.0f, 1.0f};
};

struct ScriptComponent
{
	// TODO: find way to insert OnLoad and OnUnload
	// void(*OnLoad)(ECS::Entity) = nullptr;
	// void(*OnUnload)(ECS::Entity) = nullptr;
	void(*OnUpdate)(ECS::Entity, float) = nullptr;
	void(*OnRender)(ECS::Entity) = nullptr;
};

struct Camera2DComponent
{
	Camera2D camera;
	bool is_primary = false;
};

struct RenderLayerComponent
{
	char layer;
};

struct RectangleRendererComponent
{
	Color color;
	Vector2 size;
};

struct CircleRendererComponent
{
	Color color;
	float radius;
};

struct TextureRendererComponent
{
	Texture2D texture;
	Rectangle source_rectangle;
};

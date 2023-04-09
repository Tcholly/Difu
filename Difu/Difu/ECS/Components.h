#pragma once

#include <string>
#include <raylib.h>

#include "Difu/Particles/ParticleEmitter.h"

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

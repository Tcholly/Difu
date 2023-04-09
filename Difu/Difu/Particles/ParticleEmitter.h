#pragma once

#include <vector>
#include <raylib.h>


// TODO: Add support for textures
// TODO: Add support for multiple shapes
// TODO: Add support for spawn area

struct Particle
{
    float lifetime;

    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationVelocity;

    Color startColor;
    Color endColor;
    Vector2 size;
};

struct ParticleEmitterComponent
{
	std::vector<Particle> particles;

	float spawnTimer = 0.0f;

	Vector2 spawnOffset = {0.0f, 0.0f};
	Vector2 spawnVelocity;
	Vector2 particleAcceleration;

	float centripetalAcceleration;

	float spawnRotation = 0.0f;
	float spawnRotationVelocity;
	float particleRotationAcceleration;

	Color startColor;
	Color endColor;
	Vector2 particleResolution = {1.0f, 1.0f};
	float particleMinSizeFactor = 1.0f;
	float particleMaxSizeFactor = 1.0f;

	float lifetime;
	float spawnInterval;
	float randomness;
	float spread;

	bool emitting = false;

	void StartEmitting();
	void StopEmitting();
	void ToggleEmitting();
	void EmitNow(int quantity);
	void ClearParticles();
};

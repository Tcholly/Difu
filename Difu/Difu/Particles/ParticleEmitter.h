#pragma once

#include <iostream>
#include <vector>
#include <raylib.h>

#include "Difu/Utils/Random.h"

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

class ParticleEmitter
{
public:
    ParticleEmitter();
    ParticleEmitter(Vector2 position, Vector2 velocity, Vector2 acceleration, float centripetalAcceleration, float rotation, float rotationVelocity, float rotationAcceleration, Color startColor, Color endColor, Vector2 resolution, float minSizeFactor, float maxSizeFactor, float lifetime, float interval, float randomness, float spread);

	~ParticleEmitter();

	// Update emitter
	// @param float dt afsadfsa
    void Update(float dt);
	// Render particles
    void Render();

    void StartEmitting();
    void StopEmitting();
    void ToggleEmitting();
    bool IsEmitting();
    void EmitNow(int quantity);

    Vector2 GetSpawnPosition();
    void SetSpawnPosition(Vector2 position);

    Vector2 GetSpawnVelocity();
    void SetSpawnVelocity(Vector2 velocity);

    Vector2 GetParticleAcceleration();
    void SetParticleAcceleration(Vector2 acceleration);

    float GetCentripetalAcceleration();
    void SetCentripetalAcceleration(float acceleration);

    float GetParticleSpawnRotation();
    void SetParticleSpawnRotation(float rotation);

    float GetParticleSpawnRotationVelocity();
    void SetParticleSpawnRotationVelocity(float velocity);

    float GetParticleRotationAcceleration();
    void SetParticleRotationAcceleration(float acceleration);

    float GetRandomness();
    void SetRandomness(float randomness);

    float GetSpread();
    void SetSpread(float spradValue);

    float GetParticleLifetime();
    void SetParticleLifetime(float lifetime);

    float GetSpawnInterval();
    void SetSpawnInterval(float interval);

    Color GetStartColor();
    void SetStartColor(Color color);

    Color GetEndColor();
    void SetEndColor(Color color);

    Vector2 GetParticleResolution();
    void SetParticleResolution(Vector2 resolution);

    float GetParticleMinSizeFactor();
    void SetParticleMinSizeFactor(float factor);

    float GetParticleMaxSizeFactor();
    void SetParticleMaxSizeFactor(float factor);
    
    float GetParticleCount();
	// Remove all particles
    void ClearParticles();

private:
    std::vector<Particle> particles;

    float spawnTimer = 0.0f;

	Vector2 spawnPosition;
	Vector2 spawnVelocity;
    Vector2 particleAcceleration;

	float centripetalAcceleration;

	float spawnRotation;
    float spawnRotationVelocity;
    float particleRotationAcceleration;

    Color startColor;
    Color endColor;
    Vector2 particleResolution;
    float particleMinSizeFactor;
    float particleMaxSizeFactor;

	float lifetime;
	float spawnInterval;
    float randomness;
    float spread;

    bool emitting = false;
    
    Random random;
};

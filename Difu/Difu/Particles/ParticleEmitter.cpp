#include "ParticleEmitter.h"

#include <raymath.h>
#include "Difu/Utils/MathExtension.h"
#include "Difu/Utils/Random.h"

static Random random_generator;

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	ClearParticles();
}

void ParticleEmitterComponent::StartEmitting()
{
    emitting = true;
}

void ParticleEmitterComponent::StopEmitting()
{
    emitting = false;
}

void ParticleEmitterComponent::ToggleEmitting()
{
    emitting = !emitting;
}

// TODO: If spread is 0, velocity can be opposite -> wrong
void ParticleEmitterComponent::EmitNow(int quantity)
{
    for (int i = 0; i < quantity; i++)
    {   
        Particle newParticle;
        newParticle.lifetime = lifetime;
        newParticle.position = spawnOffset;
		// Rotate velocity between -spread/2 and spread/2 from spawn velocity
        newParticle.velocity = Vector2Rotate(spawnVelocity, random_generator.GetFloat() * spread - spread / 2.0f);
		// change velocity length based on randomness
        newParticle.velocity.x += newParticle.velocity.x * randomness * random_generator.GetRangef(-2.0f, 0.0f);
        newParticle.velocity.y += newParticle.velocity.y * randomness * random_generator.GetRangef(-2.0f, 0.0f);
        newParticle.rotation = spawnRotation;
        newParticle.rotationVelocity = spawnRotationVelocity + spawnRotationVelocity * randomness * random_generator.GetRangef(-2.0f, 0.0f);
        newParticle.startColor = startColor;
        newParticle.endColor = endColor;

        float sizeFactor = random_generator.GetRangef(particleMinSizeFactor, particleMaxSizeFactor);
        newParticle.size = {particleResolution.x * sizeFactor, particleResolution.y * sizeFactor};

        particles.emplace_back(newParticle);
    }
}

void ParticleEmitterComponent::ClearParticles()
{
    particles.clear();
}

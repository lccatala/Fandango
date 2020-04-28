#include "fndgpch.h"
#include "ParticleSystem.h"
#include "Random.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include <Fandango/Renderer/Renderer2D.h>

namespace Fandango
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
	}

	void ParticleSystem::OnUpdate(TimeStep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			float life = particle.LifeRemaining / particle.LifeTime;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;

			// Fade away
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			Renderer2D::DrawQuad(particle.Position, particle.Rotation, { size, size }, color);
		}
	}
	
	void ParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = props.Position;
		particle.Rotation = Random::Float() * 360.0f;

		// Velocity
		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.LifeTime = props.LifeTime;
		particle.LifeRemaining = props.LifeTime;
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = props.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}
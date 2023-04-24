#pragma once

#include <map>
#include <string>
#include <vector>

#include "maths/vector4.h"
#include "graphics/texture.h"

class ParticleSprite;

namespace HellwatchParticle
{
	enum Type
	{
		Explosion,
		Death,
		Dash,
	};
}

class ParticleManager
{
public:

	static inline ParticleManager* ParticleManager::instance = nullptr;

	void Init();

	/// <summary>
	/// Prepares the particles
	/// </summary>
	void PrepareParticles();

	/// <summary>
	/// Plays particles at location
	/// </summary>
	void PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location);

	/// <summary>
	/// Plays particles at location
	/// </summary>
	void PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location, float duration);

	/// <summary>
	/// Plays particles at location
	/// </summary>
	void PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location, float duration, float lingerTime);

private:

	std::map<HellwatchParticle::Type, std::vector<gef::Texture*>> particlesMap;

	void LoadTexturesForParticles(HellwatchParticle::Type type, std::vector<std::string> paths);
};


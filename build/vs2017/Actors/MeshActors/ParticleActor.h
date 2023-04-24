#pragma once

#include <string>
#include <vector>
#include "../MeshActor.h"

namespace gef
{
	class Texture;
}

struct FParticleData
{
	std::vector<gef::Texture*> textures;
	float startTime = 0;
	float duration = 1;

	float lingerTime = 0.25;
	gef::Vector4 scale = gef::Vector4::kOne;
};

class ParticleActor : public MeshActor
{
public:

	/// <summary>
	/// Sets new particle data
	/// </summary>
	void SetParticleData(const FParticleData& newData) { particleData = newData; }

	/// <summary>
	/// Plays the particles
	/// </summary>
	void Play();

	/// <summary>
	/// Stops the particles
	/// </summary>
	void Stop();

	virtual void Update(float deltaTime) override;

private:

	bool bIsPlaying = false;
	bool bIsLingering = false;
	FParticleData particleData = FParticleData();

	float lingerStartTime = 0.f;
	float lastTextureTime = 0.f;
	float timeBetweenTextures = 0.f;
	int currentTextureIndex = 0;
};


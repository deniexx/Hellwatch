#include "ParticleActor.h"
#include <graphics/texture.h>

void ParticleActor::Play()
{
	bIsPlaying = true;
	lastTextureTime = SceneApp::instance->GetCurrentGameTime();
	timeBetweenTextures = particleData.duration / particleData.textures.size();

	SetMesh(SceneApp::instance->GetPrimitiveBuilder()->CreateBoxMesh(gef::Vector4(0.5f, 0.01f, 0.5f)));
	SetScale(particleData.scale);
	material.set_texture(particleData.textures[0]);
}

void ParticleActor::Stop()
{
	bIsPlaying = false;
}

void ParticleActor::Update(float deltaTime)
{
	if (bIsPlaying)
	{
		if (particleData.duration + particleData.startTime < SceneApp::instance->GetCurrentGameTime())
		{
			lingerStartTime = SceneApp::instance->GetCurrentGameTime();
			bIsLingering = true;
			Stop();
		}

		if (lastTextureTime + timeBetweenTextures < SceneApp::instance->GetCurrentGameTime())
		{
			if (++currentTextureIndex < particleData.textures.size())
			{
				material.set_texture(particleData.textures[currentTextureIndex]);
				lastTextureTime = SceneApp::instance->GetCurrentGameTime();
			}
		}
	}

	if (bIsLingering && lingerStartTime + particleData.lingerTime < SceneApp::instance->GetCurrentGameTime())
	{
		MarkForDelete();
	}
}

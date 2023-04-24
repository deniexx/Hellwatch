#include "ParticleManager.h"

#include "graphics/texture.h"
#include "Actors/MeshActors/ParticleActor.h"
#include "scene_app.h"
#include "assets/png_loader.h"
#include "graphics/image_data.h"

void ParticleManager::Init()
{
	if (!instance)
		instance = this;
	else
		delete this;
}

void ParticleManager::PrepareParticles()
{
	std::vector<std::string> explosionPaths = {
		"Assets/Explosion/explosion00.png",
		"Assets/Explosion/explosion01.png",
		"Assets/Explosion/explosion02.png",
		"Assets/Explosion/explosion03.png",
		"Assets/Explosion/explosion04.png",
		"Assets/Explosion/explosion05.png",
		"Assets/Explosion/explosion06.png",
		"Assets/Explosion/explosion07.png",
		"Assets/Explosion/explosion08.png"
	};

	std::vector<std::string> deathPaths = {
		"Assets/WhitePuff/whitePuff00.png",
		"Assets/WhitePuff/whitePuff01.png",
		"Assets/WhitePuff/whitePuff02.png",
		"Assets/WhitePuff/whitePuff03.png",
		"Assets/WhitePuff/whitePuff04.png",
		"Assets/WhitePuff/whitePuff05.png",
		"Assets/WhitePuff/whitePuff06.png",
		"Assets/WhitePuff/whitePuff07.png",
		"Assets/WhitePuff/whitePuff08.png"
	};

	std::vector<std::string> dashPaths = {
		"Assets/Flash/flash00.png",
		"Assets/Flash/flash01.png",
		"Assets/Flash/flash02.png",
		"Assets/Flash/flash03.png",
		"Assets/Flash/flash04.png",
		"Assets/Flash/flash05.png",
		"Assets/Flash/flash06.png",
		"Assets/Flash/flash07.png",
		"Assets/Flash/flash08.png"
	};

	LoadTexturesForParticles(HellwatchParticle::Explosion, explosionPaths);
	LoadTexturesForParticles(HellwatchParticle::Death, deathPaths);
	LoadTexturesForParticles(HellwatchParticle::Dash, dashPaths);
}

void ParticleManager::PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location)
{
	PlayParticlesAtLocation(type, location, 0.5f, 0.25f);
}

void ParticleManager::PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location, float duration)
{
	PlayParticlesAtLocation(type, location, duration, 0.25f);
}

void ParticleManager::PlayParticlesAtLocation(HellwatchParticle::Type type, gef::Vector4 location, float duration, float lingerTime)
{
	ParticleActor* ps = SceneApp::instance->SpawnMeshActor<ParticleActor>();

	FParticleData particleData;
	particleData.startTime = SceneApp::instance->GetCurrentGameTime();
	particleData.duration = duration;
	particleData.lingerTime = lingerTime;
	particleData.textures = particlesMap[type];

	switch (type)
	{
	case HellwatchParticle::Dash:
		particleData.scale = gef::Vector4(4.f, 4.f, 4.f);
		break;
	case HellwatchParticle::Death:
		particleData.scale = gef::Vector4(3.f, 3.f, 3.f);
		break;
	case HellwatchParticle::Explosion:
		particleData.scale = gef::Vector4(10.f, 10.f, 10.f);
		break;
	}

	ps->SetTranslation(location);
	ps->SetParticleData(particleData);
	ps->Play();
}

void ParticleManager::LoadTexturesForParticles(HellwatchParticle::Type type, std::vector<std::string> paths)
{
	gef::PNGLoader png_loader;
	std::vector<gef::Texture*> textures;
	for (const auto& text : paths)
	{
		gef::ImageData imageData;
		png_loader.Load(text.c_str(), SceneApp::instance->platform(), imageData);
		if (imageData.image() != nullptr)
		{
			gef::Texture* texture = gef::Texture::Create(SceneApp::instance->platform(), imageData);
			textures.push_back(texture);
		}
	}
	
	particlesMap[type] = textures;
}

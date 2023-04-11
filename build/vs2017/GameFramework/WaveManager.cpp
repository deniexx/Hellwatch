#include "WaveManager.h"

#include <random>
#include "Actors/MeshActors/Enemy.h"
#include "Actors/MeshActors/RangedEnemy.h"

WaveManager::WaveManager()
{
}

void WaveManager::Init()
{
	lastSpawnTime = SceneApp::instance->GetCurrentGameTime();

	FWaveDefinition wave;
	wave.maxSpawns = 5;
	wave.spawnPeriod = 6;

	waveDefinitions.push_back(wave);

	spawnLocations = {
		gef::Vector4(15.f, 0.f, 15.f),
		gef::Vector4(22.f, 0.f, 22.f),
		gef::Vector4(7.f, 0.f, 7.f),
		gef::Vector4(-7.f, 0.f, -7.f),
		gef::Vector4(-15.f, 0.f, -15.f),
		gef::Vector4(-22.f, 0.f, -22.f),
		gef::Vector4(-15.f, 0.f, 15.f),
		gef::Vector4(-22.f, 0.f, 22.f),
		gef::Vector4(-7.f, 0.f, 7.f),
		gef::Vector4(7.f, 0.f, -7.f),
		gef::Vector4(15.f, 0.f, -15.f),
		gef::Vector4(22.f, 0.f, -22.f)
	};
}

void WaveManager::StartWave()
{
	bInWave = true;
	currentWaveSpawnedEnemiesAmount = 0;
}

void WaveManager::EndWave()
{
	spawnedEnemies.clear();
	currentWave += 1;
	bInWave = false;
	//SceneApp::instance->SetGameState(GameState::Shop);
}

void WaveManager::Update()
{
	float spawnPeriod = currentWave < waveDefinitions.size() ? waveDefinitions[currentWave].spawnPeriod : SPAWN_PERIOD_AFTER25;
	float maxSpawnAmount = currentWave < waveDefinitions.size() ? waveDefinitions[currentWave].maxSpawns : USE_WAVE_FORMULA;

	bSpawnsComplete = currentWaveSpawnedEnemiesAmount == maxSpawnAmount;
	bool bCanSpawn = lastSpawnTime + spawnPeriod < SceneApp::instance->GetCurrentGameTime() && !bSpawnsComplete;

	if (bCanSpawn)
	{
		SpawnEnemy();
	}

	CheckWaveState();
}

void WaveManager::CheckWaveState()
{
	if (bSpawnsComplete)
	{
		bool bAnyEnemyAlive = false;
		for (int i = 0; i < spawnedEnemies.size(); ++i)
		{
			if (spawnedEnemies[i])
			{
				bAnyEnemyAlive = true;
				break;
			}
		}

		if (!bAnyEnemyAlive)
		{
			EndWave();
		}
	}
}

void WaveManager::SpawnEnemy()
{
	++currentWaveSpawnedEnemiesAmount;
	lastSpawnTime = SceneApp::instance->GetCurrentGameTime();
	
	bool bSpawnRanged = rand() % 255 > 140;
	int spawnLocationIndex = rand() % spawnLocations.size();

	gef::Mesh* mesh = SceneApp::instance->GetPrimitiveBuilder()->CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
	if (bSpawnRanged)
	{
		RangedEnemy* enemy = SceneApp::instance->SpawnMeshActor<RangedEnemy>(mesh, spawnLocations[spawnLocationIndex]);
		Enemy* n = (Enemy*)(enemy);
		spawnedEnemies.push_back(n);
	}
	else
	{
		Enemy* enemy = SceneApp::instance->SpawnMeshActor<Enemy>(mesh, spawnLocations[spawnLocationIndex]);
		spawnedEnemies.push_back(enemy);
	}
}

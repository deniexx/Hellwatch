#include "WaveManager.h"

#include <random>
#include "Actors/MeshActors/Enemy.h"
#include "Actors/MeshActors/RangedEnemy.h"
#include "Actors/MeshActors/Boss.h"

WaveManager::WaveManager()
{
}

void WaveManager::Init()
{
	lastSpawnTime = SceneApp::instance->GetCurrentGameTime();

	// Wave 1
	FWaveDefinition wave;
	wave.maxSpawns = 5;
	wave.spawnPeriod = 2;
	waveDefinitions.push_back(wave);

	// Wave 2
	wave.maxSpawns = 8;
	wave.spawnPeriod = 2;
	waveDefinitions.push_back(wave);

	// Wave 3
	wave.maxSpawns = 15;
	wave.spawnPeriod = 1.8f;
	waveDefinitions.push_back(wave);

	// Wave 4
	wave.maxSpawns = 15;
	wave.spawnPeriod = 1.6f;
	waveDefinitions.push_back(wave);

	// Wave 5
	wave.maxSpawns = 30;
	wave.spawnPeriod = 1.6f;
	waveDefinitions.push_back(wave);

	// Wave 6
	wave.maxSpawns = 26;
	wave.spawnPeriod = 1.4f;
	waveDefinitions.push_back(wave);

	// Wave 7
	wave.maxSpawns = 35;
	wave.spawnPeriod = 1.4f;
	waveDefinitions.push_back(wave);

	// Wave 8
	wave.maxSpawns = 50;
	wave.spawnPeriod = 1.3f;
	waveDefinitions.push_back(wave);

	// Wave 9
	wave.maxSpawns = 50;
	wave.spawnPeriod = 1.2f;
	waveDefinitions.push_back(wave);

	// Wave 10
	wave.maxSpawns = 70;
	wave.spawnPeriod = 1.f;
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

	if (currentWave == waveDefinitions.size())
	{
		SceneApp::instance->SetGameState(GameState::GameEnd);
		return;
	}

	SceneApp::instance->SetGameState(GameState::Shop);
}

void WaveManager::Update()
{
	float spawnPeriod = currentWave < waveDefinitions.size() ? waveDefinitions[currentWave].spawnPeriod : SPAWN_PERIOD_AFTER_WAVE_DEFINITIONS;
	float maxSpawnAmount = currentWave < waveDefinitions.size() ? waveDefinitions[currentWave].maxSpawns : USE_WAVE_FORMULA;

	bSpawnsComplete = currentWaveSpawnedEnemiesAmount >= maxSpawnAmount;
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
		std::vector<Enemy*> cleanEnemies;
		for (int i = 0; i < spawnedEnemies.size(); ++i)
		{
			if (!spawnedEnemies[i]->GetIsMarkedForDelete())
			{
				cleanEnemies.push_back(spawnedEnemies[i]);
			}
		}

		if (cleanEnemies.empty())
		{
			EndWave();
		}

		spawnedEnemies = cleanEnemies;
	}
}

void WaveManager::SpawnEnemy()
{
	++currentWaveSpawnedEnemiesAmount;
	lastSpawnTime = SceneApp::instance->GetCurrentGameTime();

	if (currentWave == 9 && !bBossSpawned)
	{
		boss = SceneApp::instance->SpawnMeshActor<Boss>();
		bBossSpawned = true;
	}
	
	bool bSpawnRanged = rand() % 255 > 140;
	int spawnLocationIndex = rand() % spawnLocations.size();
	int enemyClassRand = rand() % 300;

	EnemyClass::Type enemyClass;
	if (enemyClassRand < 80)
		enemyClass = EnemyClass::LIGHT;
	else if (enemyClassRand < 220)
		enemyClass = EnemyClass::NORMAL;
	else
		enemyClass = EnemyClass::HEAVY;

	gef::Mesh* mesh = SceneApp::instance->GetPrimitiveBuilder()->CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
	if (bSpawnRanged)
	{
		RangedEnemy* enemy = SceneApp::instance->SpawnMeshActor<RangedEnemy>(mesh, spawnLocations[spawnLocationIndex]);
		Enemy* n = (Enemy*)(enemy);
		n->SetClass(enemyClass);
		n->ScaleWithWave(currentWave);
		spawnedEnemies.push_back(n);
	}
	else
	{
		Enemy* enemy = SceneApp::instance->SpawnMeshActor<Enemy>(mesh, spawnLocations[spawnLocationIndex]);
		enemy->SetClass(enemyClass);
		enemy->ScaleWithWave(currentWave);
		spawnedEnemies.push_back(enemy);
	}
}

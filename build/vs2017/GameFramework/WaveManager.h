#pragma once

#include "GameFramework/Utils.h"

#define USE_WAVE_FORMULA (currentWave + currentWave * 0.5) * 5
#define SPAWN_PERIOD_AFTER25 1.f

class Enemy;
class RangedEnemy;

struct FWaveDefinition
{
	uint32_t maxSpawns;
	float spawnPeriod;
};

class WaveManager
{

public:

	WaveManager();

	void Init();

	void StartWave();
	void EndWave();

	void Update();
	void CheckWaveState();
	void SpawnEnemy();

private:

	bool bInWave = false;
	bool bSpawnsComplete = false;

	uint32_t currentWave = 0;
	uint32_t currentWaveSpawnedEnemiesAmount = 0;
	
	float lastSpawnTime = 0.f;

	std::vector<FWaveDefinition> waveDefinitions;
	std::vector<Enemy*> spawnedEnemies;
	std::vector<gef::Vector4> spawnLocations;
};

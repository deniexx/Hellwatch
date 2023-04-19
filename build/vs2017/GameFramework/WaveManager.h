#pragma once

#include "GameFramework/Utils.h"

#define USE_WAVE_FORMULA (currentWave + currentWave * 0.5) * 5
#define SPAWN_PERIOD_AFTER_WAVE_DEFINITIONS 1.f

class Enemy;
class RangedEnemy;

/// <summary>
/// A definition for a wave, the max number of spawned enemies and the period of spawning them
/// </summary>
struct FWaveDefinition
{
	uint32_t maxSpawns;
	float spawnPeriod;
};

/// <summary>
/// Manages the spawning of enemies and tracking the progress of waves
/// </summary>
class WaveManager
{

public:

	WaveManager();

	/// <summary>
	/// Must be called, initializes the wave definitions
	/// </summary>
	void Init();

	/// <summary>
	/// Starts the next wave
	/// </summary>
	void StartWave();

	/// <summary>
	/// Ends the current wave
	/// </summary>
	void EndWave();

	void Update();

	/// <summary>
	/// Checks whether all enemies have been spawned, and if any are alive
	/// </summary>
	void CheckWaveState();

	/// <summary>
	/// Spawns a random enemy and sets its class to a random one
	/// </summary>
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

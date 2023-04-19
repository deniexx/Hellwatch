#pragma once

#include "Actors/MeshActor.h"
#include "Attributes/AttributeComponent.h"
#include "ActorComponents/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "scene_app.h"
#include "DamageOnCollisionActor.h"

/// <summary>
/// Enemy class enum, used to add multipliers to stats
/// </summary>
namespace EnemyClass
{
	enum Type
	{
		HEAVY,
		NORMAL,
		LIGHT
	};
}

/// <summary>
/// A base class for all enemies, implemented as a melee enemy
/// </summary>
class Enemy : public MeshActor
{
	GENERATED_BODY(MeshActor, Enemy)

public:

	Enemy();

	/// <summary>
	/// Sets the enemy class and updates its parameters to fit the class
	/// </summary>
	virtual void SetClass(EnemyClass::Type type);

	virtual void TakeDamage(float damageAmount) override;

	/// <summary>
	/// Gets the current enemy health
	/// </summary>
	float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }

	virtual void Update(float deltaTime) override;

	virtual void OnCollision(b2Body* otherBody) override;
	
	/// <summary>
	/// Scales the enemy health, damage and money it awards with the wave
	/// </summary>
	void ScaleWithWave(int wave);

	float damageAmount;
	float attackTime;
	float attackCooldown = 2;

protected:

	virtual void PostInit() override;

	int playerMoneyToAdd = 30;
	AttributeComponent* attributes;
	CharacterMovementComponent* enemyMovement;

	/// <summary>
	/// Increases the money the player has
	/// </summary>
	void IncreasePlayerMoney();
};
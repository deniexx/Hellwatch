#pragma once

#include "Actors/MeshActor.h"
#include "Attributes/AttributeComponent.h"
#include "ActorComponents/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "scene_app.h"
#include "DamageOnCollisionActor.h"

namespace EnemyClass
{
	enum Type
	{
		HEAVY,
		NORMAL,
		LIGHT
	};
}

class Enemy : public MeshActor
{
	GENERATED_BODY(MeshActor, Enemy)

public:

	Enemy();

	virtual void SetClass(EnemyClass::Type type);
	virtual void TakeDamage(float damageAmount) override;
	float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(b2Body* otherBody) override;
	
	void ScaleWithWave(int wave);

	float damageAmount;
	float attackTime;
	float attackCooldown = 2;

protected:

	virtual void PostInit() override;

	int playerMoneyToAdd = 30;
	AttributeComponent* attributes;
	CharacterMovementComponent* enemyMovement;

	void IncreasePlayerMoney();
};
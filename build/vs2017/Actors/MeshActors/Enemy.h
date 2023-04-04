#pragma once

#include "Actors/MeshActor.h"
#include "Attributes/AttributeComponent.h"
#include "ActorComponents/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "scene_app.h"

class Enemy : public MeshActor
{
	GENERATED_BODY(MeshActor, Enemy)

public:

	virtual void TakeDamage(float damageAmount) override;
	float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
	virtual void Update(float deltaTime) override;

protected:

	AttributeComponent* attributes;
	virtual void PostInit() override;
	CharacterMovementComponent* enemyMovement;


};
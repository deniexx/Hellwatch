#pragma once

#include "Actors/MeshActor.h"
#include "Attributes/AttributeComponent.h"
#include "ActorComponents/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "scene_app.h"

class Enemy : public MeshActor
{
public:
	virtual void TakeDamage(float damageAmount);
	float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
	void update();

protected:
	AttributeComponent* attributes;
	virtual void PostInit() override;
	CharacterMovementComponent* enemyMovement;


};
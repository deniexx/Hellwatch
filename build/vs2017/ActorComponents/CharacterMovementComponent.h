#pragma once

#include "ActorComponents/ActorComponent.h"
#include "GameFramework/Utils.h"

class CharacterMovementComponent : public ActorComponent
{
	GENERATED_BODY(ActorComponent, CharacterMovementComponent)

public:

	CharacterMovementComponent();

	virtual void UpdateComponent(float deltaTime) override;

protected:
	
	virtual void PostInit() override;

private:

	b2Body* ownerCollision;
	float maximumSpeed;
	float acceleration;
	bool bFrozen;

public:
	
	void SetCollisionBody(b2Body* newBody) { ownerCollision = newBody; }
	void SetFrozen(bool bNewState) { bFrozen = bNewState; }
	void ApplyMovementForceInDirection(b2Vec2 force);


	__forceinline bool IsFrozen() const { return bFrozen; }
};


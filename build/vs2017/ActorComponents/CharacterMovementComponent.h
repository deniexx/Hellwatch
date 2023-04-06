#pragma once

#include "ActorComponents/ActorComponent.h"
#include "GameFramework/Utils.h"

class CharacterMovementComponent : public ActorComponent
{
	GENERATED_BODY(ActorComponent, CharacterMovementComponent)

public:

	CharacterMovementComponent();

	virtual void UpdateComponent(float deltaTime) override;

	/// <summary>
	/// Applies Movement force in a direction, that is scaled with the acceleration
	/// </summary>
	/// <param name="force"></param>
	void ApplyMovementForceInDirection(b2Vec2 force);

protected:
	
	virtual void PostInit() override;

private:

	b2Body* ownerCollision;
	float maximumSpeed;
	float acceleration;
	bool bFrozen;

public:
	
	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	void SetCollisionBody(b2Body* newBody) { ownerCollision = newBody; }
	void SetFrozen(bool bNewState) { bFrozen = bNewState; }
	void SetMaximumSpeed(float newMaximumSpeed) { maximumSpeed = newMaximumSpeed; }
	void SetAcceleration(float newAcceleration) { acceleration = newAcceleration; }

	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline bool IsFrozen() const { return bFrozen; }
	__forceinline float GetSpeed() const { return ownerCollision ? ownerCollision->GetLinearVelocity().Length() : 0.f; }
	__forceinline float GetMaximumSpeed() { return maximumSpeed; }
	__forceinline float GetAcceleration() { return acceleration; }
};


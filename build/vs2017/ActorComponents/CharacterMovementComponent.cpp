#include "CharacterMovementComponent.h"
#include "GameFramework/WorldObject.h"

CharacterMovementComponent::CharacterMovementComponent()
	: Super(),
	maximumSpeed(10.f),
	acceleration(75.f),
	bFrozen(false)
{
}

void CharacterMovementComponent::PostInit()
{
	ownerCollision = ownerActor->GetCollisionBody();
}

void CharacterMovementComponent::UpdateComponent(float deltaTime)
{
	if (!ownerCollision) return;

	if (bFrozen)
	{
		ownerCollision->SetLinearVelocity(b2Vec2(0, 0));
		return;
	}

	b2Vec2 velocity = ownerCollision->GetLinearVelocity();

	float currentSpeed = velocity.Length();
	if (currentSpeed > maximumSpeed)
	{
		velocity.Normalize();
		velocity *= maximumSpeed;
		ownerCollision->SetLinearVelocity(velocity);
	}
}

void CharacterMovementComponent::ApplyMovementForceInDirection(b2Vec2 force)
{
	if (ownerCollision && !bFrozen)
	{
		force *= acceleration;
		ownerCollision->ApplyForceToCenter(force, true);
	}
}

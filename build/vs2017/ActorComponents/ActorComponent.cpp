#include "ActorComponent.h"

#include "WorldObject.h"

void ActorComponent::Init(WorldObject* inOwnerActor)
{
	ownerActor = inOwnerActor;
}

void ActorComponent::PostInit()
{
}

void ActorComponent::UpdateComponent(float deltaTime)
{
}

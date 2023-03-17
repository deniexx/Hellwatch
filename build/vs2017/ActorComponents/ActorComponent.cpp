#include "ActorComponent.h"

#include "GameFramework/WorldObject.h"

void ActorComponent::Init(WorldObject* inOwnerActor)
{
	ownerActor = inOwnerActor;
	PostInit();
}

void ActorComponent::PostInit()
{
}

void ActorComponent::UpdateComponent(float deltaTime)
{
}

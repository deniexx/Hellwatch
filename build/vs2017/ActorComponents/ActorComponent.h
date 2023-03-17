#pragma once

class WorldObject;

class ActorComponent
{
public:
	
	void Init(WorldObject* inOwnerActor);
	virtual void UpdateComponent(float deltaTime);

private:

	virtual void PostInit();

	WorldObject* ownerActor;

public:

	__forceinline WorldObject* GetOwner() const { return ownerActor; }
};


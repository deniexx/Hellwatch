#pragma once

#include "Actors/MeshActor.h"
#include "Attributes/AttributeComponent.h"

class EnemyDummy : public MeshActor
{
public:

	virtual void TakeDamage(float damageAmount);

protected:

	virtual void PostInit() override;

private:

	AttributeComponent* attributes;

public:

	float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
};


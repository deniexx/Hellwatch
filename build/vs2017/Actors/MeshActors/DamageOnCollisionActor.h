#pragma once

#include "Actors/MeshActor.h"

class DamageOnCollisionActor : public MeshActor
{

public:

	virtual void OnCollision(b2Body* otherBody) override;

	void SetDamageAmount(float newAmount) { damageAmount = newAmount; }

private:

	float damageAmount = 0.f;
};


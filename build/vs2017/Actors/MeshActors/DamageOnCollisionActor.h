#pragma once

#include "Actors/MeshActor.h"

class DamageOnCollisionActor : public MeshActor
{

public:

	/// <summary>
	/// Deal damage on collision
	/// </summary>
	/// <param name="otherBody"></param>
	virtual void OnCollision(b2Body* otherBody) override;

	/// <summary>
	/// Sets the damage amount
	/// </summary>
	/// <param name="newAmount"></param>
	void SetDamageAmount(float newAmount) { damageAmount = newAmount; }

private:

	float damageAmount = 0.f;
};


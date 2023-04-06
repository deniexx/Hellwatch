#pragma once

#include "Actors/MeshActor.h"

class DamageOnCollisionActor : public MeshActor
{

public:

	/// <summary>
	/// Deal damage on collision
	/// </summary>
	virtual void OnCollision(b2Body* otherBody) override;

	/// <summary>
	/// Sets the damage amount
	/// </summary>
	void SetDamageAmount(float newAmount) { damageAmount = newAmount; }

	/// <summary>
	/// Sets to what we should apply damage
	/// </summary>
	void SetApplyDamageOn(ApplyDamageOn::Type newState) { applyDamageOn = newState; }

private:

	ApplyDamageOn::Type applyDamageOn = ApplyDamageOn::EnemiesOnly;

	float damageAmount = 0.f;
};


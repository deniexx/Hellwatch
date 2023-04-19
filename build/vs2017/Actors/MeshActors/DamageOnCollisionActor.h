#pragma once

#include "Actors/MeshActor.h"

/// <summary>
/// Actor to spawn to perform damage on collision
/// </summary>
class DamageOnCollisionActor : public MeshActor
{
	GENERATED_BODY(MeshActor, DamageOnCollisionActor)

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

	/// <summary>
	/// Specifies whether the actor should spin while moving
	/// @NOTE: This does not change the movement direction
	/// </summary>
	void SetSpinning(bool bNewState) { bSpinning = bNewState; }


	virtual void Update(float deltaTime) override;

protected:

	ApplyDamageOn::Type applyDamageOn = ApplyDamageOn::EnemiesOnly;
	bool bSpinning = false;
	float damageAmount = 0.f;
};


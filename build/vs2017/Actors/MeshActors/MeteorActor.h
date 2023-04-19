#pragma once

#include "Actors/MeshActor.h"
#include "GameFramework/Utils.h"

class MeteorActor : public MeshActor
{
	GENERATED_BODY(MeshActor, MeteorActor)

public:

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

	gef::Vector4 startPosition;
	gef::Vector4 targetPosition;

protected:

	ApplyDamageOn::Type applyDamageOn = ApplyDamageOn::EnemiesOnly;
	bool bSpinning = false;
	float damageAmount = 0.f;

	float lerpAlpha = 0.f;
};


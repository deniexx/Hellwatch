#pragma once
#include "Abilities/Ability.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "GameFramework/Utils.h"
#include "Actors/MeshActors/PlayerCharacter.h"

/// <summary>
/// Applies a force to the player in the mouse direction and makes the player briefly invincible
/// </summary>

class Dash : public Ability
{
public:
	GENERATED_BODY(Ability, Dash)

public:
	Dash();

protected:
	virtual void PostInit() override;

	virtual void Begin() override;

	virtual void End() override;
};


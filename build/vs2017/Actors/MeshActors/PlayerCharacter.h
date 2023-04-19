#pragma once

#include "Actors/MeshActor.h"
#include "GameFramework/PlayerController.h"
#include "graphics/skinned_mesh_instance.h"
#include "GameFramework/Utils.h"

class CharacterMovementComponent;
class AbilitiesComponent;
class AttributeComponent;

/// <summary>
/// The class that the player uses during the GameLoop GameState
/// </summary>
class PlayerCharacter : public MeshActor
{
	GENERATED_BODY(MeshActor, PlayerCharacter)

public:

	PlayerCharacter();

	virtual void PostInit() override;
	virtual void Update(float deltaTime) override;
	virtual void TakeDamage(float damageAmount) override;

protected:

	PlayerController* controller;

	/************************************************************************/
	/*                             KEY BINDS                                */
	/************************************************************************/
	void MoveLeft();
	void MoveRight();

	void MoveForward();
	void MoveBack();

	void ActivateAbility1();
	void ActivateAbility2();
	void ActivateAbility3();
	void ActivateAbility4();

private:

	CharacterMovementComponent* characterMovement;
	AbilitiesComponent* abilitiesComponent;
	AttributeComponent* attributes;

	void BindKeys();
	void InitializeComponents();
	void InitializeAbilitySystem();

public:

	PlayerController* GetController() const { return controller; }
	CharacterMovementComponent* GetCharacterMovement() const { return characterMovement; }
	AttributeComponent* GetCharacterAttributes() const { return attributes; }
	AbilitiesComponent* GetCharacterAbilities() const { return abilitiesComponent; }

};


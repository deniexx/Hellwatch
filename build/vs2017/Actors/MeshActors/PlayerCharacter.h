#pragma once

#include "Actors/MeshActor.h"
#include "GameFramework/PlayerController.h"
#include "graphics/skinned_mesh_instance.h"
#include "GameFramework/Utils.h"
#include "graphics/sprite.h"

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

	/// <summary>
	/// Applies invincibility for specified duration
	/// </summary>
	void ApplyInvincibilityForDuration(float invincibilityDuration);

	void DrawPlayerHUD();

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

	void OnControllerLeftStick(gef::Vector2 delta);
	void OnControllerActivateAbility1(gef::Vector2 delta);
	void OnControllerActivateAbility2(gef::Vector2 delta);
	void OnControllerActivateAbility3(gef::Vector2 delta);
	void OnControllerActivateAbility4(gef::Vector2 delta);
	void OnControllerInvertUpDownMovement(gef::Vector2 delta);

private:

	CharacterMovementComponent* characterMovement;
	AbilitiesComponent* abilitiesComponent;
	AttributeComponent* attributes;

	/* Used to track when should player invincibility end */
	float invincibilityEndTime = 0.f;

	void BindKeys();
	void InitializeComponents();
	void InitializeAbilitySystem();

	bool bInvertMovement = false;

	std::vector<gef::Sprite> abilitySprites;
	std::vector<gef::Sprite> backgroundSprites;
	gef::Sprite background;

public:

	PlayerController* GetController() const { return controller; }
	CharacterMovementComponent* GetCharacterMovement() const { return characterMovement; }
	AttributeComponent* GetCharacterAttributes() const { return attributes; }
	AbilitiesComponent* GetCharacterAbilities() const { return abilitiesComponent; }

};


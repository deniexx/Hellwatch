#pragma once

#include "Actors/MeshActor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Utils.h"

class CharacterMovementComponent;

class PlayerCharacter : public MeshActor
{
	GENERATED_BODY(MeshActor, PlayerCharacter)

public:

	PlayerCharacter();

	virtual void PostInit() override;

	virtual void Update(float deltaTime) override;

protected:

	PlayerController* controller;

	void MoveLeft();
	void MoveRight();

	void MoveForward();
	void MoveBack();

private:

	CharacterMovementComponent* characterMovement;

public:

	CharacterMovementComponent* GetCharacterMovement() const { return characterMovement; }

};


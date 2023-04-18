#pragma once
#include "Enemy.h"
#include <maths/vector4.h>

class Boss : public Enemy
{
	GENERATED_BODY(Enemy, Boss)

public:
	Boss();
	virtual void Update(float deltaTime) override;
	virtual void Slam(float deltaTime);
	virtual void Jump(float deltaTime);

protected:

	AttributeComponent* attributes;
	virtual void PostInit() override;
	CharacterMovementComponent* bossMovement;

	bool isJumping = false;
	bool pastMidpoint = false;

	gef::Vector4 playerPosition;
	gef::Vector4 bossPosition;
	gef::Vector4 midpoint;
	float alpha = 0;

};


#pragma once
#include "Enemy.h"
class Boss : public Enemy
{
public:
	Boss();
	virtual void Update(float deltaTime) override;
	virtual void Slam(float deltaTime);

protected:
	AttributeComponent* attributes;
	virtual void PostInit() override;
	CharacterMovementComponent* bossMovement;


};


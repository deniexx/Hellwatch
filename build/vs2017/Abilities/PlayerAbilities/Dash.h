#pragma once
#include "Abilities/Ability.h"
#include "GameFramework/Utils.h"

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


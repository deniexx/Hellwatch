#pragma once

#include "Abilities/Ability.h"
#include "GameFramework/Utils.h"


class PiercingStrike : public Ability
{
	GENERATED_BODY(Ability, PiercingStrike)

protected:

	virtual void PostInit() override;

	virtual void Begin() override;

	virtual void End() override;
};


#pragma once

#include "Abilities/Ability.h"
#include "GameFramework/Utils.h"

class Meteor : public Ability
{
public:

	GENERATED_BODY(Ability, Meteor)

public:

	Meteor();


protected:

	virtual void PostInit() override;

	virtual void Begin() override;

	virtual void End() override;
};


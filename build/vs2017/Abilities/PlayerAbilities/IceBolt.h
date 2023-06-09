#pragma once

#include "Abilities/Ability.h"
#include "GameFramework/Utils.h"

/// <summary>
/// Fires an ice projectile that hits the first target ONLY
/// </summary>
class IceBolt : public Ability
{
public:

	GENERATED_BODY(Ability, IceBolt)

public:
	IceBolt();


protected:

	virtual void PostInit() override;

	virtual void Begin() override;

	virtual void End() override;
};


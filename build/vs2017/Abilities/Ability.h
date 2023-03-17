#pragma once

#include <string>
#include "Attributes/AttributeTypes.h"

class AbilitiesComponent;

namespace AbilityActivationKey
{
	enum ActivationKey
	{
		AbilityKey1 = 0,
		AbilityKey2,
		AbilityKey3,
		AbilityKey4,

		None
	};
}

namespace CooldownStartPolicy
{
	enum StartPolicy
	{
		OnAbilityStart,
		OnAbilityEnd
	};
}

struct ActivationOutcome
{
	std::string abilityName;
	std::string failReason;
	bool bWasSuccessful = 0;

	AbilityActivationKey::ActivationKey keyOfAbility = AbilityActivationKey::None;
};

class Ability
{
public:

	void Init(AbilitiesComponent* inOwningComponent);
	virtual void Update(float deltaTime);

	ActivationOutcome TryActivate();
	bool EndAbility();

protected:

	virtual void PostInit();
	virtual void Begin();
	virtual void End();
	virtual bool CanActivate(ActivationOutcome& OutActivationOutcome);

private:

	AbilityActivationKey::ActivationKey activationKey;

	float cooldownTimer;
	float cooldown;
	uint32_t abilityCostAmount;
	HellwatchAttribute::Attribute abilityCostType;
	CooldownStartPolicy::StartPolicy cooldownStartPolicy;

	std::string abilityName;
	bool bIsActive;
	AbilitiesComponent* owningComponent;

public:

	void SetActivationKey(AbilityActivationKey::ActivationKey key) { activationKey = key; }
	void SetAbilityCostAmount(uint32_t newCost) { abilityCostAmount = newCost; }
	void SetAbilityCostType(HellwatchAttribute::Attribute newCost) { abilityCostType = newCost; }
	void SetAbilityName(std::string newName) { abilityName = newName; }
	void SetIsActive(bool bNewState) { bIsActive = bNewState; }
	void SetCooldownStartPolicy(CooldownStartPolicy::StartPolicy newPolicy) { cooldownStartPolicy = newPolicy; }
	void SetAbilityCooldown(float newCooldown);

	virtual bool IsActive() { return bIsActive; };

	__forceinline AbilityActivationKey::ActivationKey GetActivationKey() const { return activationKey; }
	__forceinline const std::string& GetAbilityName() const { return abilityName; }
	__forceinline bool IsInCooldown() const { return cooldown > 0 && cooldownTimer > 0.f; }

	AbilitiesComponent* GetOwner() const { return owningComponent; }
};


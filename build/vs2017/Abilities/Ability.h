#pragma once

#include <string>
#include "Attributes/AttributeTypes.h"

class AbilitiesComponent;

/// <summary>
/// The key that will be used to activate this ability 1, 2, 3, 4
/// </summary>
namespace AbilityActivationKey
{
	enum Type
	{
		AbilityKey1 = 0,
		AbilityKey2,
		AbilityKey3,
		AbilityKey4,

		None
	};
}

/// <summary>
/// When should cooldown start, on start of ability or end of ability
/// </summary>
namespace CooldownStartPolicy
{
	enum Type
	{
		OnAbilityStart,
		OnAbilityEnd
	};
}

/// <summary>
/// Outcome of attempting to activate this ability
/// </summary>
struct ActivationOutcome
{
	std::string abilityName;
	std::string failReason;
	bool bWasSuccessful = 0;

	AbilityActivationKey::Type keyOfAbility = AbilityActivationKey::None;
};

/// <summary>
/// A base ability class, designed to be inherited and provided with functionality
/// </summary>
class Ability
{
public:

	/// <summary>
	/// Initializes the ability with the ability component
	/// </summary>
	void Init(AbilitiesComponent* inOwningComponent);

	virtual void Update(float deltaTime);

	/// <summary>
	/// Attempts to activate this ability, returns an ActivationOutcome struct
	/// </summary>
	ActivationOutcome TryActivate();

	/// <summary>
	/// Attempts to end the ability, returns a bool whether it was successful
	/// </summary>
	bool EndAbility();

protected:

	/// <summary>
	/// Called after ability has been initialized, do set up here
	/// </summary>
	virtual void PostInit();

	/// <summary>
	/// Begins the ability, called if TryActivate was successful
	/// </summary>
	virtual void Begin();

	/// <summary>
	/// Called on end of ability
	/// </summary>
	virtual void End();

	/// <summary>
	/// Can the ability be activated
	/// </summary>
	/// <param name="OutActivationOutcome">This is an out parameter, holding information whether we can activate this ability, and if we can't why</param>
	/// <returns>A boolean, whether we can activate this abiltiy or not</returns>
	virtual bool CanActivate(ActivationOutcome& OutActivationOutcome);

private:

	AbilityActivationKey::Type activationKey;

	float cooldownTimer;
	float cooldown;
	uint32_t abilityCostAmount;
	HellwatchAttribute::Type abilityCostType;
	CooldownStartPolicy::Type cooldownStartPolicy;

	std::string abilityName;
	bool bIsActive;
	AbilitiesComponent* owningComponent;

public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	void SetActivationKey(AbilityActivationKey::Type key) { activationKey = key; }
	void SetAbilityCostAmount(uint32_t newCost) { abilityCostAmount = newCost; }
	void SetAbilityCostType(HellwatchAttribute::Type newCost) { abilityCostType = newCost; }
	void SetAbilityName(std::string newName) { abilityName = newName; }
	void SetIsActive(bool bNewState) { bIsActive = bNewState; }
	void SetCooldownStartPolicy(CooldownStartPolicy::Type newPolicy) { cooldownStartPolicy = newPolicy; }
	void SetAbilityCooldown(float newCooldown);

	/// <summary>
	/// Checks if the ability is active
	/// </summary>
	virtual bool IsActive() { return bIsActive; };

	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline AbilityActivationKey::Type GetActivationKey() const { return activationKey; }
	__forceinline const std::string& GetAbilityName() const { return abilityName; }
	__forceinline bool IsInCooldown() const { return cooldown > 0 && cooldownTimer > 0.f; }

	AbilitiesComponent* GetOwner() const { return owningComponent; }
};


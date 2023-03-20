#pragma once

#include "ActorComponent.h"
#include "Abilities/Ability.h"

#include <vector>
#include <string>

class AttributeComponent;

class AbilitiesComponent : public ActorComponent
{

public:

	/// <summary>
	/// Adds an ability to our system
	/// @NOTE: It does not equip it, just adds it to the available abilities vector
	/// </summary>
	void AddAbility(Ability* abilityToAdd);

	/// <summary>
	/// Equips an ability, at a specific key
	/// </summary>
	void EquipAbility(std::string abilityName, AbilityActivationKey::Type keyToEquipAt);

	/// <summary>
	/// Attempts to activate ability by key
	/// </summary>
	void ActivateAbilityByKey(AbilityActivationKey::Type key);

	/// <summary>
	/// Attempts to activate ability by its name
	/// @NOTE: Preferred to use ActivateAbilityByKey version
	/// </summary>
	void ActivateAbilityByName(std::string name);

	/// <summary>
	/// Ends an ability by key, we can use this if we prematurely end ability, etc...
	/// </summary>
	void EndAbilityByKey(AbilityActivationKey::Type key);

	/// <summary>
	/// Ends an ability by name, we can use this if we prematurely end ability, etc...
	/// Preferred to use EndAbilityByKey version
	/// </summary>
	/// <param name="name"></param>
	void EndAbilityByName(std::string name);

	/// <summary>
	/// Registers an attribute component to this ability system
	/// </summary>
	void RegisterAttributes(AttributeComponent* newAttributes);

	virtual void UpdateComponent(float deltaTime) override;

protected:

	virtual void PostInit() override;

private:

	std::vector<Ability*> availableAbilities;
	Ability* equippedAbilities[4];
	AttributeComponent* attributes;

	bool HasAbility(Ability* abilityToCheck);
	bool HasAbility(const std::string& abilityToCheck);
	bool IsAbilityEquipped(Ability* abilityToCheck);
	Ability* FindAbility(const std::string& abilityName);
	void EquipAbilityInternal(Ability* abilityToEquip, AbilityActivationKey::Type keyToEquipAt);

public:

	AttributeComponent* GetAttributeComponent() const { return attributes; }

};


#pragma once

#include "ActorComponent.h"
#include "Abilities/Ability.h"

#include <vector>
#include <string>

class AttributeComponent;

class AbilitiesComponent : public ActorComponent
{

public:

	void AddAbility(Ability* abilityToAdd);
	void EquipAbility(Ability* abilityToEquip, AbilityActivationKey::ActivationKey keyToEquipAt);

	void ActivateAbilityByKey(AbilityActivationKey::ActivationKey key);
	void ActivateAbilityByName(std::string name);
	void EndAbilityByKey(AbilityActivationKey::ActivationKey key);
	void EndAbilityByName(std::string name);
	void RegisterAttributes(AttributeComponent* newAttributes);

	virtual void UpdateComponent(float deltaTime) override;

protected:

	virtual void PostInit() override;

private:

	std::vector<Ability*> availableAbilities;
	Ability* equippedAbilities[4];
	AttributeComponent* attributes;

public:

	AttributeComponent* GetAttributeComponent() const { return attributes; }

};


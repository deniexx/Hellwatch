#include "AbilitiesComponent.h"

void AbilitiesComponent::PostInit()
{
	for (int i = 0; i < 4; ++i)
		equippedAbilities[i] = nullptr;
}

void AbilitiesComponent::UpdateComponent(float deltaTime)
{
	for (const auto& ability : equippedAbilities)
	{
		if (ability && (ability->IsActive() || ability->IsInCooldown()))
			ability->Update(deltaTime);
	}
}

void AbilitiesComponent::AddAbility(Ability* abilityToAdd)
{
	availableAbilities.push_back(abilityToAdd);
}

void AbilitiesComponent::EquipAbility(Ability* abilityToEquip, AbilityActivationKey::Type keyToEquipAt)
{
	equippedAbilities[keyToEquipAt] = abilityToEquip;
}

void AbilitiesComponent::ActivateAbilityByKey(AbilityActivationKey::Type key)
{
	if (equippedAbilities[key])
		equippedAbilities[key]->TryActivate();
}

void AbilitiesComponent::ActivateAbilityByName(std::string name)
{
	for (const auto& ability : equippedAbilities)
	{
		if (!ability) continue;

		if (name == ability->GetAbilityName())
			ability->TryActivate();
	}
}

void AbilitiesComponent::EndAbilityByKey(AbilityActivationKey::Type key)
{
	if (equippedAbilities[key])
		equippedAbilities[key]->EndAbility();
}

void AbilitiesComponent::EndAbilityByName(std::string name)
{
	for (const auto& ability : equippedAbilities)
	{
		if (!ability) continue;

		if (name == ability->GetAbilityName())
			ability->EndAbility();
	}
}

void AbilitiesComponent::RegisterAttributes(AttributeComponent* newAttributes)
{
	attributes = newAttributes;
}

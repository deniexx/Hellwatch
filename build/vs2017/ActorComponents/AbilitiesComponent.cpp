#include "AbilitiesComponent.h"

void AbilitiesComponent::PostInit()
{
	for (int i = 0; i < 4; ++i)
		equippedAbilities[i] = nullptr;
}

bool AbilitiesComponent::HasAbility(Ability* abilityToCheck)
{
	for (auto ability : availableAbilities)
	{
		if (!ability) continue;
		if (ability->GetAbilityName() == abilityToCheck->GetAbilityName())
			return true;
	}

	return false;
}

bool AbilitiesComponent::HasAbility(const std::string& abilityToCheck)
{
	for (auto ability : availableAbilities)
	{
		if (!ability) continue;
		if (ability->GetAbilityName() == abilityToCheck)
			return true;
	}

	return false;
}

bool AbilitiesComponent::IsAbilityEquipped(Ability* abilityToCheck)
{
	for (auto ability : equippedAbilities)
	{
		if (!ability) continue;
		if (ability->GetAbilityName() == abilityToCheck->GetAbilityName())
			return true;
	}

	return false;
}

Ability* AbilitiesComponent::FindAbility(const std::string& abilityName)
{
	for (auto ability : availableAbilities)
	{
		if (!ability) continue;
		if (ability->GetAbilityName() == abilityName)
			return ability;
	}

	return nullptr;
}

void AbilitiesComponent::EquipAbilityInternal(Ability* abilityToEquip, AbilityActivationKey::Type keyToEquipAt)
{
	Ability* prevAbility = equippedAbilities[keyToEquipAt];
	if (prevAbility)
		prevAbility->SetActivationKey(AbilityActivationKey::None);

	equippedAbilities[keyToEquipAt] = abilityToEquip;
	abilityToEquip->SetActivationKey(keyToEquipAt);
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
	abilityToAdd->Init(this);
	availableAbilities.push_back(abilityToAdd);
}

void AbilitiesComponent::EquipAbility(std::string abilityName, AbilityActivationKey::Type keyToEquipAt)
{
	if (!HasAbility(abilityName)) return;
	if (Ability* foundAbility = FindAbility(abilityName))
	{
		if (IsAbilityEquipped(foundAbility)) return;

		EquipAbilityInternal(foundAbility, keyToEquipAt);
	}
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

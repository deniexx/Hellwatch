#include "Ability.h"

#include "ActorComponents/ActorComponent.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "Attributes/AttributeComponent.h"
#include "GameFramework/WorldObject.h"

void Ability::Init(AbilitiesComponent* inOwningComponent)
{
    owningComponent = inOwningComponent;

    SetAbilityCostAmount(0);
    SetActivationKey(AbilityActivationKey::None);
    SetAbilityCostType(HellwatchAttribute::None);
    SetAbilityName("");
    SetIsActive(false);
    SetAbilityCooldown(0);
    SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);

    PostInit();
}

void Ability::Update(float deltaTime)
{
    if (cooldown > 0 && cooldownTimer > 0)
        cooldownTimer -= deltaTime;

}

ActivationOutcome Ability::TryActivate()
{
    ActivationOutcome outcome;

    if (!CanActivate(outcome))
    {
        return outcome;
    }

    Begin();
    return outcome;
}

bool Ability::EndAbility()
{
    if (!IsActive())
        return false;

    End();
    return true;
}

void Ability::PostInit()
{
}

void Ability::Begin()
{
    if (cooldownStartPolicy == CooldownStartPolicy::OnAbilityStart)
        cooldownTimer = cooldown;
}

void Ability::End()
{
    if (cooldownStartPolicy == CooldownStartPolicy::OnAbilityEnd)
        cooldownTimer = cooldown;
}

bool Ability::CanActivate(ActivationOutcome& OutActivationOutcome)
{
	OutActivationOutcome.abilityName = abilityName;
	OutActivationOutcome.keyOfAbility = activationKey;

    if (!GetOwner())
    {
        OutActivationOutcome.bWasSuccessful = false;
        OutActivationOutcome.failReason = "No Ability System Owner found for this Ability!";
        return false;
    }

    if (IsActive())
    {
        OutActivationOutcome.bWasSuccessful = false;
        OutActivationOutcome.failReason = "Ability is already active!";
        return false;
    }

    AttributeComponent* attributes = GetOwner()->GetAttributeComponent();

    if (!attributes->CheckHasEnoughOfAttributeByType(abilityCostType, abilityCostAmount))
    {
        float amountMissing = abilityCostAmount - attributes->GetCurrentAttributeValueByType(abilityCostType);

		OutActivationOutcome.bWasSuccessful = false;
		OutActivationOutcome.failReason = sprintf("Not enough %s owned! Missing %f.01", AttributeToString(abilityCostType).c_str(), amountMissing);
        return false;
    }

    OutActivationOutcome.bWasSuccessful = true;
    return true;
}

void Ability::SetAbilityCooldown(float newCooldown)
{
    cooldown = newCooldown;
    cooldown = 0.f;
}

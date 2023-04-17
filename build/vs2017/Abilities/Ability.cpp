#include "Ability.h"

#include "ActorComponents/ActorComponent.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "Attributes/AttributeComponent.h"
#include "GameFramework/WorldObject.h"

Ability::Ability()
{
    cooldownTimer = 0.f;
    cooldown = 0.f;
    abilityCostType = HellwatchAttribute::None;
    abilityCostAmount = 0.f;
    bIsActive = false;
    abilityName = "";
    bIsTargeted = false;
    bIsTargeting = false;
    cooldownStartPolicy = CooldownStartPolicy::OnAbilityStart;
}

void Ability::Init(AbilitiesComponent* inOwningComponent)
{
    owningComponent = inOwningComponent;
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

    if (IsAbilityTargeting())
    {
        CommitAbility();
    }

    if (!CanActivate(outcome))
    {
        return outcome;
    }

    Begin();
    return outcome;
}

void Ability::CommitAbility()
{
    if (bIsTargeting)
    {
        EndTargeting();
        Commit();
    }
}

bool Ability::EndAbility()
{
	EndTargeting();

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

    if (IsAbilityTargeted())
	{
		SetIsTargeting(true);
        BeginTargeting();
        return;
    }  
}

void Ability::End()
{
    if (cooldownStartPolicy == CooldownStartPolicy::OnAbilityEnd)
        cooldownTimer = cooldown;

    EndTargeting();
}

void Ability::Commit()
{
}

void Ability::BeginTargeting()
{
}

void Ability::EndTargeting()
{
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

    if (cooldownTimer > 0.f)
    {
        OutActivationOutcome.bWasSuccessful = false;
        OutActivationOutcome.failReason = "Ability is in cooldown";
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
    cooldownTimer = 0.f;
}

float Ability::GetDamageAmount()
{
    AttributeComponent* attributes = GetOwnerAttributes();

    if (attributes)
    {
        float multipliyer = (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Wisdom) * 0.1f) + 1;
        damageAmount *= multipliyer;
    }

    return damageAmount;
}

AttributeComponent* Ability::GetOwnerAttributes()
{
    return GetOwner() ? GetOwner()->GetAttributeComponent() : nullptr;
}

#include "Dash.h"

Dash::Dash()
	: Super()
{
}

void Dash::PostInit() 
{
	SetAbilityName("Dash");
	SetAbilityCostType(HellwatchAttribute::Mana);
	SetAbilityCostAmount(10.f);
	SetAbilityCooldown(3.0f);
	SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);
}

void Dash::Begin() {
	Super::Begin();

}

void Dash::End() {

}
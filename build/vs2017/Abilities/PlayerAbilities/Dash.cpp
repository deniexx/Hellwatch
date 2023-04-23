#include "Dash.h"

Dash::Dash()
	: Super()
{
}

void Dash::PostInit() 
{
	SetAbilityName("Dash");
	SetAbilityCostType(HellwatchAttribute::Mana);
	SetAbilityCostAmount(15.f);
	SetAbilityCooldown(5.0f);
	SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);
}

void Dash::Begin() {
	Super::Begin();

	if (PlayerCharacter* player = SceneApp::instance->GetPlayerCharacter()) 
	{
		gef::Vector4 mousePos = ProjectScreenToWorldSpace(SceneApp::instance->GetLastTouchPosition());
		gef::Vector4 translation = GetOwner()->GetOwner()->GetTranslation();
		gef::Vector4 target = mousePos - translation;
		target.Normalise();

		b2Vec2 forceDir = b2Vec2(target.x(), target.z());
		forceDir *= 5000.f;

		player->ApplyInvincibilityForDuration(2);
		player->GetCollisionBody()->ApplyForceToCenter(forceDir, true);
	}

	SceneApp::instance->PlaySample("Dash");

	GetOwnerAttributes()->ApplyAttributeChange(abilityCostType, -abilityCostAmount);
}

void Dash::End() {

}
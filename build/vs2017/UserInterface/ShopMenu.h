#pragma once

#include "Menu.h"
#include "Attributes/AttributeTypes.h"
#include "Abilities/Ability.h"

#define UPGRADE_COST_FORMULA(amountOfUpgrades) (upgradeCost * amountOfUpgrades) * 0.5 + 5;

class ShopMenu : public Menu
{
public:

	virtual void Init();
	virtual void Update() override;
	virtual void DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_) override;

protected:

	virtual void OnMouseButtonPressed(gef::Vector2 mousePos);

private:

	uint32_t upgradeCost = 10;
	int upgradesAmount = 0;

	void IncreaseVitality();
	void IncreaseWisdom();
	void IncreaseToughness();
	void IncreaseEnergy();
	void CloseShop();

	void IncreaseAttribute(HellwatchAttribute::Type attribute);
	void IncreaseUpgradeCost();
	bool DoesPlayerHaveMoneyForUpgrade();
	//void GrantAbility(Ability* abilityToGrant);
};


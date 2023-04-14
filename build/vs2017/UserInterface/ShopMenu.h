#pragma once

#include "Menu.h"
#include "Attributes/AttributeTypes.h"
#include "Abilities/Ability.h"

class ShopMenu : public Menu
{
public:

	virtual void Init();

protected:

	virtual void OnMouseButtonPressed(gef::Vector2 mousePos);

private:

	void IncreaseVitality();
	void IncreaseWisdom();
	void IncreaseToughness();
	void IncreaseEnergy();
	void CloseShop();

	void IncreaseAttribute(HellwatchAttribute::Type attribute);
	void GrantAbility(Ability* abilityToGrant);
};


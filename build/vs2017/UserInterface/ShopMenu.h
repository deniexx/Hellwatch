#pragma once

#include "Menu.h"
#include "Attributes/AttributeTypes.h"
#include "Abilities/Ability.h"

#define UPGRADE_COST_FORMULA(amountOfUpgrades) (upgradeCost * amountOfUpgrades) * 0.5 + 5
#define MAX_COST 3000.f

/// <summary>
/// Shop menu to be used in Shop GameState
/// </summary>
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

	/************************************************************************/
	/*                       Attribute Increasers                           */
	/************************************************************************/
	void IncreaseVitality();
	void IncreaseWisdom();
	void IncreaseToughness();
	void IncreaseEnergy();

	/// <summary>
	/// Closes the shop
	/// </summary>
	void CloseShop();

	/// <summary>
	/// Increases an attribute by 1
	/// </summary>
	/// <param name="attribute"></param>
	void IncreaseAttribute(HellwatchAttribute::Type attribute);

	/// <summary>
	/// Increases the upgrade cost
	/// </summary>
	void IncreaseUpgradeCost();

	/// <summary>
	/// Checks if the player has enough money for the current upgrade
	/// </summary>
	/// <returns></returns>
	bool DoesPlayerHaveMoneyForUpgrade();

	/* Not used currently */
	//void GrantAbility(Ability* abilityToGrant);
};


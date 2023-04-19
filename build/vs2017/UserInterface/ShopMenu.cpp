#include "ShopMenu.h"
#include "GameFramework/PlayerController.h"
#include "graphics/sprite.h"
#include "assets/png_loader.h"
#include <graphics/image_data.h>
#include "graphics/texture.h"
#include "Actors/MeshActors/PlayerCharacter.h"
#include "Attributes/AttributeComponent.h"

void ShopMenu::Init()
{
	// Set up buttons
	MenuButton increaseAttributeButton;
	increaseAttributeButton.buttonText = "Vitality +";
	increaseAttributeButton.color = 0xFF0000FF;
	increaseAttributeButton.justification = gef::TJ_LEFT;
	increaseAttributeButton.scale = 3.f;
	increaseAttributeButton.position = gef::Vector4(160.f, 400.f, 0.f);
	increaseAttributeButton.callbackFunction = bindFunc(IncreaseVitality);
	menuButtons.push_back(increaseAttributeButton);

	increaseAttributeButton.buttonText = "Wisdom +";
	increaseAttributeButton.position = gef::Vector4(160.f, 600.f, 0.f);
	increaseAttributeButton.callbackFunction = bindFunc(IncreaseWisdom);
	menuButtons.push_back(increaseAttributeButton);

	increaseAttributeButton.buttonText = "Toughness +";
	increaseAttributeButton.position = gef::Vector4(1300.f, 400.f, 0.f);
	increaseAttributeButton.callbackFunction = bindFunc(IncreaseToughness);
	menuButtons.push_back(increaseAttributeButton);

	increaseAttributeButton.buttonText = "Energy +";
	increaseAttributeButton.position = gef::Vector4(1300.f, 600.f, 0.f);
	increaseAttributeButton.callbackFunction = bindFunc(IncreaseEnergy);
	menuButtons.push_back(increaseAttributeButton);

	increaseAttributeButton.buttonText = "Close Shop";
	increaseAttributeButton.position = gef::Vector4(750.f, 900.f, 0.f);
	increaseAttributeButton.callbackFunction = bindFunc(CloseShop);
	menuButtons.push_back(increaseAttributeButton);

	// Create controller, so that we can navigate the menu
	menuController = new PlayerController(SceneApp::instance->platform());
	FKeyBindKeyboard keyboardKeybind;

	// Move down on menu keybinds
	keyboardKeybind.inputAction = HellwatchInputAction::Released;
	keyboardKeybind.keyCode = gef::Keyboard::KC_S;
	keyboardKeybind.functionBind = bindFunc(OnDownButtonPressed);
	menuController->BindKeyboardEvent(keyboardKeybind);

	keyboardKeybind.keyCode = gef::Keyboard::KC_DOWN;
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Move up on menu keybinds
	keyboardKeybind.keyCode = gef::Keyboard::KC_UP;
	keyboardKeybind.functionBind = bindFunc(OnUpButtonPressed);
	menuController->BindKeyboardEvent(keyboardKeybind);

	keyboardKeybind.keyCode = gef::Keyboard::KC_W;
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Submit keybind
	keyboardKeybind.keyCode = gef::Keyboard::KC_RETURN;
	keyboardKeybind.functionBind = bindFunc(PressMenuButton);
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Mouse click to submit keybind
	FKeyBindMouse mouseKeybind;
	mouseKeybind.clickAction = gef::TT_NEW;
	mouseKeybind.functionBind = bindFunc_ONEParam(OnMouseButtonPressed, gef::Vector2, mousePos);
	menuController->BindMouseEvent(mouseKeybind);
}

void ShopMenu::Update()
{
	Menu::Update();

	menuButtons[0].buttonText = "+ Vitality (" + std::to_string((int)SceneApp::instance->GetPlayerCharacter()->GetCharacterAttributes()->GetCurrentAttributeValueByType(HellwatchAttribute::Vitality)) + ")";
	menuButtons[1].buttonText = "+ Wisdom (" + std::to_string((int)SceneApp::instance->GetPlayerCharacter()->GetCharacterAttributes()->GetCurrentAttributeValueByType(HellwatchAttribute::Wisdom)) + ")";
	menuButtons[2].buttonText = "+ Toughness (" + std::to_string((int)SceneApp::instance->GetPlayerCharacter()->GetCharacterAttributes()->GetCurrentAttributeValueByType(HellwatchAttribute::Toughness)) + ")";
	menuButtons[3].buttonText = "+ Energy (" + std::to_string((int)SceneApp::instance->GetPlayerCharacter()->GetCharacterAttributes()->GetCurrentAttributeValueByType(HellwatchAttribute::Energy)) + ")";
}

void ShopMenu::DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_)
{
	Menu::DrawMenuHUD(font_, sprite_renderer_);

	std::string moneyAmount = "Money: " + std::to_string(SceneApp::instance->GetPlayerMoney());
	std::string upgradeCostStr = "Cost: " + std::to_string(upgradeCost);
	font_->RenderText(sprite_renderer_, gef::Vector4(100, 200, 0, 0), 1, 0xFF00FFFF, gef::TJ_LEFT, moneyAmount.c_str());
	font_->RenderText(sprite_renderer_, gef::Vector4(100, 250, 0, 0), 1, 0xFF00FFFF, gef::TJ_LEFT, upgradeCostStr.c_str());
}

void ShopMenu::OnMouseButtonPressed(gef::Vector2 mousePos)
{
	for (const auto& button : menuButtons)
	{
		if (menuController)
		{
			if (mousePos.x > button.position.x() - 20 && mousePos.x < button.position.x() + 250 && mousePos.y > button.position.y() && mousePos.y < button.position.y() + 90)
			{
				button.callbackFunction();
				break;
			}
		}
	}
}

void ShopMenu::IncreaseVitality()
{
	IncreaseAttribute(HellwatchAttribute::Vitality);
}

void ShopMenu::IncreaseWisdom()
{
	IncreaseAttribute(HellwatchAttribute::Wisdom);
}

void ShopMenu::IncreaseToughness()
{
	IncreaseAttribute(HellwatchAttribute::Toughness);
}

void ShopMenu::IncreaseEnergy()
{
	IncreaseAttribute(HellwatchAttribute::Energy);
}

void ShopMenu::CloseShop()
{
	SceneApp::instance->SetGameState(GameState::GameLoop);
}

void ShopMenu::IncreaseAttribute(HellwatchAttribute::Type attribute)
{
	if (!DoesPlayerHaveMoneyForUpgrade())
	{
		return;
	}

	SceneApp::instance->ApplyCostToPlayerMoney(upgradeCost);
	IncreaseUpgradeCost();

	PlayerCharacter* pc = SceneApp::instance->GetPlayerCharacter();
	AttributeComponent* attributes = pc ? pc->GetCharacterAttributes() : nullptr;

	if (attributes)
	{
		attributes->ApplyAttributeChange(attribute, 1);
	}
}

void ShopMenu::IncreaseUpgradeCost()
{
	++upgradesAmount;
	upgradeCost = UPGRADE_COST_FORMULA(upgradesAmount);

	if (upgradeCost > MAX_COST)
		upgradeCost = MAX_COST;
}

bool ShopMenu::DoesPlayerHaveMoneyForUpgrade()
{
	return SceneApp::instance->GetPlayerMoney() > upgradeCost;
}

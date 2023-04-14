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

	menuSprite = new gef::Sprite();
	gef::PNGLoader png_loader;
	gef::ImageData imageData;
	png_loader.Load("Assets/ShopMenu.png", SceneApp::instance->platform(), imageData);
	if (imageData.image() != nullptr)
	{
		gef::Texture* texture = gef::Texture::Create(SceneApp::instance->platform(), imageData);
		menuSprite->set_position(SceneApp::instance->platform().width() / 2, SceneApp::instance->platform().height() / 2, 0.f);
		menuSprite->set_width(1920);
		menuSprite->set_height(1080);
		menuSprite->set_texture(texture);
	}

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
	PlayerCharacter* pc = SceneApp::instance->GetPlayerCharacter();
	AttributeComponent* attributes = pc ? pc->GetCharacterAttributes() : nullptr;

	if (attributes)
	{
		attributes->ApplyAttributeChange(attribute, 1);
	}
}

void ShopMenu::GrantAbility(Ability* abilityToGrant)
{

}

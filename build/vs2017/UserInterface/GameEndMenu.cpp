#include "GameEndMenu.h"

#include "scene_app.h"
#include "GameFramework/PlayerController.h"
#include "Actors/MeshActors/PlayerCharacter.h"
#include "Attributes/AttributeComponent.h"
#include "GameFramework/Utils.h"
#include "input/sony_controller_input_manager.h"

void GameEndMenu::Init()
{
	bool bPlayerWon = SceneApp::instance->GetPlayerCharacter()->GetCharacterAttributes()->GetCurrentAttributeValueByType(HellwatchAttribute::Health) > 0;
	
	menuButtons.clear();

	// Set up buttons
	MenuButton mainMenuButton;
	mainMenuButton.buttonText = "Main Menu";
	mainMenuButton.color = 0xFF0000FF;
	mainMenuButton.justification = gef::TJ_LEFT;
	mainMenuButton.scale = 3.f;
	mainMenuButton.position = gef::Vector4(160.f, 540.f, 0.f);
	mainMenuButton.callbackFunction = bindFunc(OnMainMenuButtonClicked);
	menuButtons.push_back(mainMenuButton);

	MenuButton exitButton;
	exitButton.buttonText = "Exit";
	exitButton.color = 0xFF0000FF;
	exitButton.justification = gef::TJ_LEFT;
	exitButton.scale = 3.f;
	exitButton.position = gef::Vector4(1560.f, 540.f, 0.f);
	exitButton.callbackFunction = bindFunc(OnExitButtonClicked);
	menuButtons.push_back(exitButton);

	if (bPlayerWon)
	{
		MenuButton continueButton;
		continueButton.buttonText = "Continue";
		continueButton.justification = gef::TJ_LEFT;
		continueButton.scale = 3.f;
		continueButton.position = gef::Vector4(780.f, 540.f, 0.f);
		continueButton.callbackFunction = bindFunc(OnContinueButtonClicked);
		menuButtons.push_back(continueButton);
	}

	gameEndText = bPlayerWon ? "You won!" : "You lost!";

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

	// Controller events
	FKeyBindController controllerKeybind;
	controllerKeybind.inputAction = HellwatchInputAction::Pressed;
	controllerKeybind.keyCode = gef_SONY_CTRL_DOWN;
	controllerKeybind.functionBind = bindFunc_ONEParam(OnControllerDownButton, gef::Vector2, dir);
	menuController->BindControllerEvent(controllerKeybind);

	controllerKeybind.keyCode = gef_SONY_CTRL_UP;
	controllerKeybind.functionBind = bindFunc_ONEParam(OnControllerUpButton, gef::Vector2, dir);
	menuController->BindControllerEvent(controllerKeybind);

	controllerKeybind.keyCode = gef_SONY_CTRL_CROSS;
	controllerKeybind.functionBind = bindFunc_ONEParam(OnControllerSubmitButton, gef::Vector2, dir);
	menuController->BindControllerEvent(controllerKeybind);
}

void GameEndMenu::DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_)
{
	Menu::DrawMenuHUD(font_, sprite_renderer_);
	font_->RenderText(sprite_renderer_, gef::Vector4(780, 100, 0), 3.f, 0xFF0000FF, gef::TJ_LEFT, gameEndText.c_str());
}

void GameEndMenu::OnMainMenuButtonClicked()
{
	SceneApp::instance->SetGameState(GameState::MainMenu);
}

void GameEndMenu::OnExitButtonClicked()
{
	exit(0);
}

void GameEndMenu::OnContinueButtonClicked()
{
	SceneApp::instance->SetGameState(GameState::GameLoop);
}

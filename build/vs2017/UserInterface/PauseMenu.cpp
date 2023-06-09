#include "PauseMenu.h"
#include "GameFramework/PlayerController.h"
#include "input/sony_controller_input_manager.h"


void PauseMenu::Init()
{
	// Set up buttons
	MenuButton pauseMenuButton;
	pauseMenuButton.buttonText = "Resume";
	pauseMenuButton.color = 0xFF0000FF;
	pauseMenuButton.justification = gef::TJ_CENTRE;
	pauseMenuButton.scale = 3.f;
	pauseMenuButton.position = gef::Vector4(960.f, 340.f, 0.f);
	pauseMenuButton.callbackFunction = bindFunc(ResumeGame);
	menuButtons.push_back(pauseMenuButton);

	pauseMenuButton.buttonText = "Main Menu";
	pauseMenuButton.position = gef::Vector4(960.f, 640.f, 0.f);
	pauseMenuButton.callbackFunction = bindFunc(ExitToMainMenu);
	menuButtons.push_back(pauseMenuButton);

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

void PauseMenu::ResumeGame()
{
	SceneApp::instance->SetGameState(GameState::GameLoop);
}

void PauseMenu::ExitToMainMenu()
{
	SceneApp::instance->SetGameState(GameState::MainMenu);
}

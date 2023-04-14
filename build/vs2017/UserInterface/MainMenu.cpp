#include "MainMenu.h"

#include "GameFramework/PlayerController.h"
#include "graphics/sprite.h"
#include "assets/png_loader.h"
#include <graphics/image_data.h>
#include "graphics/texture.h"

void MainMenu::Init()
{
	MenuButton startButton;
	startButton.buttonText = "Start";
	startButton.color = 0xFF0000FF;
	startButton.justification = gef::TJ_LEFT;
	startButton.scale = 3.f;
	startButton.position = gef::Vector4(160.f, 540.f, 0.f);
	startButton.callbackFunction = bindFunc(OnStartButtonClicked);
	menuButtons.push_back(startButton);

	MenuButton exitButton;
	exitButton.buttonText = "Exit";
	exitButton.color = 0xFF0000FF;
	exitButton.justification = gef::TJ_LEFT;
	exitButton.scale = 3.f;
	exitButton.position = gef::Vector4(160.f, 640.f, 0.f);
	exitButton.callbackFunction = bindFunc(OnExitButtonClicked);
	menuButtons.push_back(exitButton);

	menuSprite = new gef::Sprite();
	gef::PNGLoader png_loader;
	gef::ImageData imageData;
	png_loader.Load("Assets/MainMenu.png", SceneApp::instance->platform(), imageData);
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

void MainMenu::OnStartButtonClicked()
{
	SceneApp::instance->SetGameState(GameState::GameLoop);
}

void MainMenu::OnExitButtonClicked()
{
	exit(0);
}
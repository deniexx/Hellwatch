#pragma once

#include "GameFramework/Utils.h"
#include <future>

/// <summary>
/// A Menu button definition, used for rendering and callbacks
/// </summary>
struct MenuButton
{
	gef::Vector4 position;
	uint32_t color = 0xffffffff;
	float scale = 1;
	gef::TextJustification justification;
	std::string buttonText;
	std::function<void()> callbackFunction;
};

/// <summary>
/// Base Menu class, that mainly handles actions that we bind to in child classes
/// </summary>
class Menu
{

public:
	/*******************************************************
	*					MENU INPUT                         *
	/*******************************************************/

	~Menu();

	/// <summary>
	/// Initializes the menu
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// Updates the controller of the menu and checks for highlights
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Draws the menu HUD
	/// </summary>
	virtual void DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_);

	PlayerController* menuController;
	std::vector<MenuButton> menuButtons;
	int currentButtonFocused = 0;
	gef::Sprite* menuSprite;

protected:

	/// <summary>
	/// When the player presses a button to navigate  up the menu
	/// </summary>
	void OnUpButtonPressed();

	/// <summary>
	/// When the player presses a button to navigate down the menu
	/// </summary>
	void OnDownButtonPressed();

	/// <summary>
	/// When the player presses the mouse button, checks if the mouse is over any buttons and if it is, it activates it
	/// </summary>
	virtual void OnMouseButtonPressed(gef::Vector2 mousePos);

	/// <summary>
	/// When the player presses a button to navigate  up the menu
	/// </summary>
	void OnControllerUpButton(gef::Vector2 dir);

	/// <summary>
	/// When the player presses a button to navigate down the menu
	/// </summary>
	void OnControllerDownButton(gef::Vector2 dir);

	/// <summary>
	/// When the player presses a button to navigate down the menu
	/// </summary>
	void OnControllerSubmitButton(gef::Vector2 dir);

	/// <summary>
	/// Checks whether to highlight any buttons
	/// </summary>
	void CheckForHighlight();

	/// <summary>
	/// Presses the currently selected menu button
	/// </summary>
	void PressMenuButton();
};


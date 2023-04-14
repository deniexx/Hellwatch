#pragma once

#include "GameFramework/Utils.h"
#include <future>

struct MenuButton
{
	gef::Vector4 position;
	uint32_t color = 0xffffffff;
	float scale = 1;
	gef::TextJustification justification;
	std::string buttonText;
	std::function<void()> callbackFunction;
};

class Menu
{

public:
	/*******************************************************
	*					MENU INPUT                         *
	/*******************************************************/

	~Menu();

	virtual void Init() = 0;
	virtual void Update();

	PlayerController* menuController;
	virtual void DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_);
	std::vector<MenuButton> menuButtons;
	int currentButtonFocused = 0;
	gef::Sprite* menuSprite;

protected:

	void OnDownButtonPressed();
	void OnUpButtonPressed();
	virtual void OnMouseButtonPressed(gef::Vector2 mousePos);
	void OnControllerDownButton(gef::Vector2 dir);
	void OnControllerUpButton(gef::Vector2 dir);
	void CheckForHighlight();
	void PressMenuButton();


};


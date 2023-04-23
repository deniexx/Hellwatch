#pragma once

#include "Menu.h"

class GameEndMenu : public Menu
{
public:

	virtual void Init();

	virtual void DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_) override;

private:

	/// <summary>
	/// Goes back to the main menu once the button has been clicked
	/// </summary>
	void OnMainMenuButtonClicked();

	/// <summary>
	/// Exits the game once the exit button has been clicked
	/// </summary>
	void OnExitButtonClicked();

	/// <summary>
	/// Continues the game if the player has won and has clicked this button
	/// </summary>
	void OnContinueButtonClicked();

	std::string gameEndText;
};


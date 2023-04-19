#pragma once

#include "Menu.h"

/// <summary>
/// Main Menu of the game
/// </summary>
class MainMenu : public Menu
{
public:

	virtual void Init();

private:

	/// <summary>
	/// Starts the GameLoop once start button has been clicked
	/// </summary>
	void OnStartButtonClicked();

	/// <summary>
	/// Exits the game once the exit button has been clicked
	/// </summary>
	void OnExitButtonClicked();
};


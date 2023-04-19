#pragma once

#include "Menu.h"

/// <summary>
/// Pause Menu to be used in Pause GameState
/// </summary>
class PauseMenu : public Menu
{
public:

	virtual void Init() override;

private:

	/// <summary>
	/// Resumes the game
	/// </summary>
	void ResumeGame();

	/// <summary>
	/// Exits to main menu
	/// </summary>
	void ExitToMainMenu();
};


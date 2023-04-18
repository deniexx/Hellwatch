#pragma once

#include "Menu.h"

class PauseMenu : public Menu
{
public:

	virtual void Init() override;

private:

	void ResumeGame();

	void ExitToMainMenu();
};


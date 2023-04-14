#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:

	virtual void Init();

private:

	void OnStartButtonClicked();
	void OnExitButtonClicked();
};


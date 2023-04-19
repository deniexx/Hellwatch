#include "Menu.h"
#include "GameFramework/PlayerController.h"

Menu::~Menu()
{
	delete menuController;
	menuController = NULL;
}

void Menu::Update()
{
	if (menuController)
	{
		menuController->Update();
		CheckForHighlight();
	}
}

void Menu::DrawMenuHUD(gef::Font* font_, gef::SpriteRenderer* sprite_renderer_)
{
	for (int i = 0; i < menuButtons.size(); ++i)
	{
		float btnScale = i == currentButtonFocused ? menuButtons[i].scale * 1.2f : menuButtons[i].scale;
		font_->RenderText(sprite_renderer_, menuButtons[i].position, btnScale, menuButtons[i].color, menuButtons[i].justification, menuButtons[i].buttonText.c_str());
	}
}

void Menu::OnUpButtonPressed()
{
	currentButtonFocused = (currentButtonFocused - 1) % menuButtons.size();
}

void Menu::OnDownButtonPressed()
{
	currentButtonFocused = (currentButtonFocused + 1) % menuButtons.size();
}

void Menu::OnMouseButtonPressed(gef::Vector2 mousePos)
{
	for (const auto& button : menuButtons)
	{
		if (mousePos.x > button.position.x() - 20 && mousePos.x < button.position.x() + 180 && mousePos.y > button.position.y() && mousePos.y < button.position.y() + 90)
		{
			button.callbackFunction();
		}
	}
}

void Menu::OnControllerUpButton(gef::Vector2 dir)
{
	OnUpButtonPressed();
}

void Menu::OnControllerDownButton(gef::Vector2 dir)
{
	OnDownButtonPressed();
}

void Menu::CheckForHighlight()
{
	if (menuController)
	{
		gef::Vector2 mousePos = menuController->GetMousePosition();
		
		for (int i = 0; i < menuButtons.size(); ++i)
		{
			if (mousePos.x > menuButtons[i].position.x() - 20 && mousePos.x < menuButtons[i].position.x() + 180 && mousePos.y > menuButtons[i].position.y() && mousePos.y < menuButtons[i].position.y() + 90)
			{
				currentButtonFocused = i;
				break;
			}
		}
	}
}

void Menu::PressMenuButton()
{
	menuButtons[currentButtonFocused].callbackFunction();
}

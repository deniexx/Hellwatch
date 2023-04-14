#include "Menu.h"
#include "GameFramework/PlayerController.h"

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
	gef::Vector2 mousePos = menuController->GetMousePosition();
	font_->RenderText(sprite_renderer_, gef::Vector4(1700.0f, 1000.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Mouse: %.1f, %.1f", mousePos.x, mousePos.y);

	for (int i = 0; i < menuButtons.size(); ++i)
	{
		float btnScale = i == currentButtonFocused ? menuButtons[i].scale * 1.2f : menuButtons[i].scale;
		font_->RenderText(sprite_renderer_, menuButtons[i].position, btnScale, menuButtons[i].color, menuButtons[i].justification, menuButtons[i].buttonText.c_str());
	}
}

void Menu::OnDownButtonPressed()
{
	currentButtonFocused = (currentButtonFocused + 1) % menuButtons.size();
}

void Menu::OnUpButtonPressed()
{
	currentButtonFocused = (currentButtonFocused - 1) % menuButtons.size();
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

void Menu::OnControllerDownButton(gef::Vector2 dir)
{
	currentButtonFocused = (currentButtonFocused + 1) % menuButtons.size();
}


void Menu::OnControllerUpButton(gef::Vector2 dir)
{
	currentButtonFocused = (currentButtonFocused - 1) % menuButtons.size();
}

void Menu::CheckForHighlight()
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

void Menu::PressMenuButton()
{
	menuButtons[currentButtonFocused].callbackFunction();
}

#include "PlayerController.h"

#include <system/application.h>

// Input includes
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>

PlayerController::PlayerController(gef::Platform& platform)
{
	inputManager = gef::InputManager::Create(platform);
	touchPosition = gef::Vector2(0.f, 0.f);
	activeTouchID = -1;
}

gef::Vector2 PlayerController::GetMousePosition()
{
	if (gef::TouchInputManager* touchInput = inputManager->touch_manager())
	{
		return touchInput->mouse_position();
	}

	return gef::Vector2::kZero;
}

bool PlayerController::BindKeyboardEvent(const FKeyBindKeyboard keyboardEvent, bool bUnique, bool bReplaceIfUnique)
{
	bool bSuccessful = false;

	if (keyboardEvent.functionBind != nullptr)
	{
		bool bKeyReplaced = false;
		bool bCanBind = true;
		if (bUnique)
		{
			for (int i = 0 ; i < keyboardKeyBinds.size(); ++i)
			{
				if (keyboardKeyBinds[i].keyCode == keyboardEvent.keyCode)
				{
					if (!bReplaceIfUnique)
					{
						bCanBind = false;
					}
					else
					{
						if (bKeyReplaced)
						{
							keyboardKeyBinds.erase(keyboardKeyBinds.begin() + i - 1);
						}
						else
						{
							keyboardKeyBinds[i] = keyboardEvent;
							bKeyReplaced = true;
							bSuccessful = true;
						}
					}
				}
			}
		}

		if (bCanBind)
		{
			keyboardKeyBinds.push_back(keyboardEvent);
			bSuccessful = true;
		}
		else bSuccessful = false;
	}

	return bSuccessful;
}

bool PlayerController::BindControllerEvent(const FKeyBindController controllerEvent, bool bUnique, bool bReplaceIfUnique)
{
	bool bSuccessful = false;

	if (controllerEvent.functionBind != nullptr)
	{
		bool bKeyReplaced = false;
		bool bCanBind = true;
		if (bUnique)
		{
			for (int i = 0; i < controllerKeyBinds.size(); ++i)
			{
				if (controllerKeyBinds[i].keyCode == controllerEvent.keyCode || controllerKeyBinds[i].axis == controllerEvent.axis)
				{
					if (!bReplaceIfUnique)
					{
						bCanBind = false;
					}
					else
					{
						if (bKeyReplaced)
						{
							controllerKeyBinds.erase(controllerKeyBinds.begin() + i - 1);
						}
						else
						{
							controllerKeyBinds[i] = controllerEvent;
							bKeyReplaced = true;
							bSuccessful = true;
						}
					}
				}
			}
		}

		if (bCanBind)
		{
			controllerKeyBinds.push_back(controllerEvent);
			bSuccessful = true;
		}
		else bSuccessful = false;
	}

	return bSuccessful;
}

bool PlayerController::BindMouseEvent(const FKeyBindMouse mouseEvent)
{
	if (mouseEvent.functionBind == nullptr)
		return false;

	mouseKeyBinds.push_back(mouseEvent);
	return true;
}

void PlayerController::BindKeyBindSet(FKeyBindSet keyBindSet)
{
	keyboardKeyBinds.clear();
	controllerKeyBinds.clear();
	mouseKeyBinds.clear();

	for (const auto& keybind : keyBindSet.keyboardKeyBinds)
		BindKeyboardEvent(keybind);

	for (const auto& keybind : keyBindSet.controllerKeyBinds)
		BindControllerEvent(keybind);

	for (const auto& keybind : keyBindSet.mouseKeyBinds)
		BindMouseEvent(keybind);
}

void PlayerController::Update()
{
	inputManager->Update();

	if (gef::Keyboard* keyboard = inputManager->keyboard())
	{
		EvaluateKeyboardKeybinds(keyboard);
	}

	if (const gef::SonyController* controller = inputManager->controller_input()->GetController(0))
	{
		EvaluateControllerKeybinds(controller);
	}

	if (const gef::TouchInputManager* touchInput = inputManager->touch_manager())
	{
		if (touchInput->max_num_panels() > 0)
			EvaluateMouseKeybinds(touchInput);
	}
}

void PlayerController::EvaluateKeyboardKeybinds(gef::Keyboard* keyboard)
{
	for (const auto& keybind : keyboardKeyBinds)
	{
		switch (keybind.inputAction)
		{
			case HellwatchInputAction::Pressed:
			{
				if (keyboard->IsKeyPressed(keybind.keyCode))
					keybind.functionBind();
			}
			break;
			case HellwatchInputAction::Held:
			{
				if (keyboard->IsKeyDown(keybind.keyCode))
					keybind.functionBind();
			}
			break;
			case HellwatchInputAction::Released:
			{
				if (keyboard->IsKeyReleased(keybind.keyCode))
					keybind.functionBind();
			}
			break;
		}
	}
}

void PlayerController::EvaluateControllerKeybinds(const gef::SonyController* controller)
{
	for (const auto& keybind : controllerKeyBinds)
	{
		if (keybind.axis != HellwatchControllerAxis::None)
		{
			switch (keybind.axis)
			{
				case HellwatchControllerAxis::LeftStickXY:
				{
					keybind.functionBind(gef::Vector2(controller->left_stick_x_axis(), controller->left_stick_y_axis()));
				}
				break;
				case HellwatchControllerAxis::RightStickXY:
				{
					keybind.functionBind(gef::Vector2(controller->right_stick_x_axis(), controller->right_stick_y_axis()));
				}
				break;
			}
		}
		else if (keybind.inputAction != HellwatchInputAction::None)
		{
			for (const auto& keybind : controllerKeyBinds)
			{
				switch (keybind.inputAction)
				{
					case HellwatchInputAction::Pressed:
					{
						if (controller->buttons_pressed() & keybind.keyCode)
							keybind.functionBind(gef::Vector2(0.f, 0.f));
					}
					break;
					case HellwatchInputAction::Held:
					{
						if (controller->buttons_down() & keybind.keyCode)
							keybind.functionBind(gef::Vector2(0.f, 0.f));
					}
					break;
					case HellwatchInputAction::Released:
					{
						if (controller->buttons_released() & keybind.keyCode)
							keybind.functionBind(gef::Vector2(0.f, 0.f));
					}
					break;
				}
			}
		}
	}
}

void PlayerController::EvaluateMouseKeybinds(const gef::TouchInputManager* touchInput)
{
	const gef::TouchContainer& panel_touches = touchInput->touches(0);

	// go through the touches
	for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
	{
		// if active touch id is -1, then we are not currently processing a touch
		if (activeTouchID == -1)
		{
			// check for the start of a new touch
			if (touch->type == gef::TT_NEW)
			{
				activeTouchID = touch->id;

				// do any processing for a new touch here
				// we're just going to record the position of the touch
				touchPosition = touch->position;

				for (const auto& mousebind : mouseKeyBinds)
				{
					if (mousebind.clickAction == gef::TT_NEW)
						mousebind.functionBind(touchPosition);
				}
			}
		}
		else if (activeTouchID == touch->id)
		{
			// we are processing touch data with a matching id to the one we are looking for
			if (touch->type == gef::TT_ACTIVE)
			{
				// update an active touch here
				// we're just going to record the position of the touch
				touchPosition = touch->position;

				for (const auto& mousebind : mouseKeyBinds)
				{
					if (mousebind.clickAction == gef::TT_ACTIVE)
						mousebind.functionBind(touchPosition);
				}
			}
			else if (touch->type == gef::TT_RELEASED)
			{
				// the touch we are tracking has been released
				// perform any actions that need to happen when a touch is released here
				// we're not doing anything here apart from resetting the active touch id
				activeTouchID = -1;

				for (const auto& mousebind : mouseKeyBinds)
				{
					if (mousebind.clickAction == gef::TT_RELEASED)
						mousebind.functionBind(touchPosition);
				}
			}
		}
	}
}

const FKeyBindSet& PlayerController::GetKeybindSet() const
{
	FKeyBindSet keybindSet;

	keybindSet.keyboardKeyBinds = keyboardKeyBinds;
	keybindSet.controllerKeyBinds = controllerKeyBinds;
	keybindSet.mouseKeyBinds = mouseKeyBinds;

	return keybindSet;
}

#pragma once

#include <cstdint>
#include <functional>
#include <input/touch_input_manager.h>
#include <input/keyboard.h>
#include "Utils.h"

namespace gef
{
	class InputManager;
	class Platform;
	class SonyController;
}

namespace HellwatchInputAction
{
	enum Type
	{
		None = 0,
		Pressed,
		Held,
		Released
	};
}

namespace HellwatchControllerAxis
{
	enum Type
	{
		None = 0,
		LeftStickXY,
		RightStickXY
	};
}

/* -------------------------------------- */
/*            Key bind structs            */
/* -------------------------------------- */
struct FKeyBindKeyboard
{
	HellwatchInputAction::Type inputAction;
	gef::Keyboard::KeyCode keyCode;
	std::function<void()> functionBind;
};

struct FKeyBindController
{
	int keyCode;
	HellwatchInputAction::Type inputAction = HellwatchInputAction::None;
	HellwatchControllerAxis::Type axis = HellwatchControllerAxis::None;
	std::function<void(gef::Vector2)> functionBind;
};

struct FKeyBindMouse
{
	gef::TouchType clickAction;
	std::function<void(gef::Vector2)> functionBind;
};

struct FKeyBindSet
{
	std::vector<FKeyBindKeyboard> keyboardKeyBinds;
	std::vector<FKeyBindController> controllerKeyBinds;
	std::vector<FKeyBindMouse> mouseKeyBinds;
};

/// <summary>
/// Responsible for reading input and calling appropriate events
/// </summary>
class PlayerController
{
public:
	PlayerController(gef::Platform& platform);

public:

	/// <summary>
	/// Gets the mouse Position
	/// </summary>
	/// <returns>The mouse position on the screen</returns>
	gef::Vector2 GetMousePosition();
	
	/// <summary>
	/// Binds a keyboard event to this player controller
	/// </summary>
	/// <param name="keyboardEvent">The event to be bound</param>
	/// <param name="bUnique">Should the event be unique, i.e. no other events bound to the same key</param>
	/// <param name="bReplaceIfUnique">Should we replace if the event is not unique</param>
	/// <returns>True, if the event was successfully bound</returns>
	bool BindKeyboardEvent(const FKeyBindKeyboard keyboardEvent, bool bUnique = false, bool bReplaceIfUnique = false);

	/// <summary>
	/// Binds a controller event to this player controller
	/// </summary>
	/// <param name="controllerEvent">The event to be bound</param>
	/// <param name="bUnique">Should the event be unique, i.e. no other events bound to the same key</param>
	/// <param name="bReplaceIfUnique">Should we replace if the event is not unique</param>
	/// <returns>True, if the event was successfully bound</returns>
	bool BindControllerEvent(const FKeyBindController controllerEvent, bool bUnique = false, bool bReplaceIfUnique = false);

	/// <summary>
	/// Binds a mouse event to this player controller
	/// </summary>
	/// <param name="mouseEvent">The event to be bound</param>
	/// <returns>True, if the event was successfully bound</returns>
	bool BindMouseEvent(const FKeyBindMouse mouseEvent);

	/// <summary>
	/// Binds a full key bind set and removes ALL previous key binds
	/// </summary>
	/// <param name ="keyBindSet">The key bind set to be bound</param>
	void BindKeyBindSet(FKeyBindSet keyBindSet);

	/// <summary>
	/// Updates the player controller and processes input
	/// </summary>
	void Update();

private:
	/* -------------------------------------- */
	/*      Process key binds functions       */ 
	/* -------------------------------------- */

	void EvaluateKeyboardKeybinds(gef::Keyboard* keyboard);
	void EvaluateControllerKeybinds(const gef::SonyController* controller);
	void EvaluateMouseKeybinds(const gef::TouchInputManager* touchInput);

	/* -------------------------------------- */
	/*               Key Binds                */
	/* -------------------------------------- */
	std::vector<FKeyBindKeyboard> keyboardKeyBinds;
	std::vector<FKeyBindController> controllerKeyBinds;
	std::vector<FKeyBindMouse> mouseKeyBinds;

	/* -------------------------------------- */
	/*               Mouse Stuff              */
	/* -------------------------------------- */
	Int32 activeTouchID;
	gef::Vector2 touchPosition;

	/* -------------------------------------- */
	/*             Input Manager              */
	/* -------------------------------------- */
	gef::InputManager* inputManager;

public:

	const FKeyBindSet& GetKeybindSet() const;
};


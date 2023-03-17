#include "PlayerCharacter.h"
#include "ActorComponents/CharacterMovementComponent.h"

PlayerCharacter::PlayerCharacter()
	: Super()
{
	meshName = "";
}

void PlayerCharacter::PostInit()
{
	controller = new PlayerController(SceneApp::instance->platform());
	FKeyBindKeyboard keybind;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.keyCode = gef::Keyboard::KC_A;
	keybind.functionBind = bindFunc(MoveLeft);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_D;
	keybind.functionBind = bindFunc(MoveRight);
	controller->BindKeyboardEvent(keybind);


	keybind.keyCode = gef::Keyboard::KC_W;
	keybind.functionBind = bindFunc(MoveForward);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_S;
	keybind.functionBind = bindFunc(MoveBack);
	controller->BindKeyboardEvent(keybind);

	b2BodyDef bodyDef;
	bodyDef.position.Set(0.f, 0.f);
	bodyDef.type = b2_dynamicBody;
	bodyDef.enabled = true;
	bodyDef.linearDamping = 5.f;

	b2PolygonShape shape;
	shape.SetAsBox(1.f, 1.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;

	SetCollisionBody(SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, this));

	characterMovement = new CharacterMovementComponent();
	characterMovement->Init(this);
}

void PlayerCharacter::Update(float deltaTime)
{
	controller->Update();
	characterMovement->UpdateComponent(deltaTime);

	b2Body* body = GetCollisionBody();

	if (body)
	{
		b2Vec2 translation = body->GetPosition();
		SetTranslation(gef::Vector4(translation.x, 0, translation.y));
	}
}

void PlayerCharacter::MoveLeft()
{
	if (characterMovement)
		characterMovement->ApplyMovementForceInDirection(b2Vec2(1, 0));
}

void PlayerCharacter::MoveRight()
{
	if (characterMovement)
		characterMovement->ApplyMovementForceInDirection(b2Vec2(-1, 0));
}

void PlayerCharacter::MoveForward()
{
	if (characterMovement)
		characterMovement->ApplyMovementForceInDirection(b2Vec2(0, 1));
}

void PlayerCharacter::MoveBack()
{
	if (characterMovement)
		characterMovement->ApplyMovementForceInDirection(b2Vec2(0, -1));
}

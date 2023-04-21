#include "PlayerCharacter.h"
#include "ActorComponents/CharacterMovementComponent.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "Attributes/AttributeComponent.h"
#include "Abilities/PlayerAbilities/IceBolt.h"
#include "Abilities/PlayerAbilities/Dash.h"
#include "Abilities/PlayerAbilities/PiercingStrike.h"
#include "Abilities/PlayerAbilities/Meteor.h"

PlayerCharacter::PlayerCharacter()
	: Super()
{
	meshName = "Ganfaul";
	ID = PLAYER_ID;
}

void PlayerCharacter::PostInit()
{
	SetMesh(SceneApp::instance->RequestMeshByName(meshName));
	gef::Material mat;
	if (gef::Texture* texture = SceneApp::instance->RequestTextureByName("Ganfaul"))
		mat.set_texture(texture);

	SetMaterial(mat);

	BindKeys();
	InitializeComponents();
}

void PlayerCharacter::BindKeys()
{
	controller = new PlayerController(SceneApp::instance->platform());
	FKeyBindKeyboard keybind;
	keybind.inputAction = HellwatchInputAction::Held;
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

	keybind.keyCode = gef::Keyboard::KC_1;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.functionBind = bindFunc(ActivateAbility1);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_2;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.functionBind = bindFunc(ActivateAbility2);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_3;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.functionBind = bindFunc(ActivateAbility3);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_4;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.functionBind = bindFunc(ActivateAbility4);
	controller->BindKeyboardEvent(keybind);

	keybind.keyCode = gef::Keyboard::KC_ESCAPE;
	keybind.inputAction = HellwatchInputAction::Pressed;
	keybind.functionBind = [this]() {SceneApp::instance->SetGameState(GameState::PauseMenu); };
	controller->BindKeyboardEvent(keybind);
}

void PlayerCharacter::InitializeComponents()
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.f, 0.f);
	bodyDef.type = b2_dynamicBody;
	bodyDef.enabled = true;
	bodyDef.linearDamping = 5.f;

	b2PolygonShape shape;
	shape.SetAsBox(.75f, .75f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;

	SetCollisionBody(SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, this));

	characterMovement = CreateComponent<CharacterMovementComponent>();
	characterMovement->Init(this);

	InitializeAbilitySystem();
}

void PlayerCharacter::InitializeAbilitySystem()
{
	attributes = CreateComponent<AttributeComponent>();

	FAttribute attributeToAdd;
	attributeToAdd.attributeType = HellwatchAttribute::Health;
	attributeToAdd.bClampedToZero = true;
	attributeToAdd.maxAmount = 10.f;
	attributeToAdd.currentAmount = 10.f;
	attributes->AddAttribute(attributeToAdd);

	attributeToAdd.attributeType = HellwatchAttribute::Mana;
	attributes->AddAttribute(attributeToAdd);

	attributeToAdd.attributeType = HellwatchAttribute::Vitality;
	attributeToAdd.currentAmount = 0;
	attributeToAdd.maxAmount = 32;
	attributes->AddAttribute(attributeToAdd);

	attributeToAdd.attributeType = HellwatchAttribute::Wisdom;
	attributes->AddAttribute(attributeToAdd);

	attributeToAdd.attributeType = HellwatchAttribute::Toughness;
	attributes->AddAttribute(attributeToAdd);

	attributeToAdd.attributeType = HellwatchAttribute::Energy;
	attributes->AddAttribute(attributeToAdd);

	abilitiesComponent = CreateComponent<AbilitiesComponent>();
	abilitiesComponent->Init(this);
	abilitiesComponent->RegisterAttributes(attributes);
	
	IceBolt* iceBolt = new IceBolt();
	abilitiesComponent->AddAbility(iceBolt);
	abilitiesComponent->EquipAbility("IceBolt", AbilityActivationKey::AbilityKey1);

	Dash* dash = new Dash();
	abilitiesComponent->AddAbility(dash);
	abilitiesComponent->EquipAbility("Dash", AbilityActivationKey::AbilityKey2);

	PiercingStrike* piercingStrike = new PiercingStrike();
	abilitiesComponent->AddAbility(piercingStrike);
	abilitiesComponent->EquipAbility("Piercing Strike", AbilityActivationKey::AbilityKey3);

	Meteor* meteor = new Meteor();
	abilitiesComponent->AddAbility(meteor);
	abilitiesComponent->EquipAbility("Meteor", AbilityActivationKey::AbilityKey4);
}

void PlayerCharacter::Update(float deltaTime)
{
	Super::Update(deltaTime);

	controller->Update();
}

void PlayerCharacter::DrawPlayerHUD()
{
	gef::Font* font = SceneApp::instance->GetFont();
	gef::SpriteRenderer* spriteRenderer = SceneApp::instance->GetSpriteRenderer();

	std::string healthText = "Heath: " + std::to_string((int)attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health));
	std::string manaText = "Mana: " + std::to_string((int)attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Mana));

	font->RenderText(spriteRenderer, gef::Vector4(10, 10, 0), 1, 0xFF0000FF, gef::TJ_LEFT, healthText.c_str());
	font->RenderText(spriteRenderer, gef::Vector4(10, 40, 0), 1, 0xFFFF0000, gef::TJ_LEFT, manaText.c_str());
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

void PlayerCharacter::ActivateAbility1()
{
	abilitiesComponent->ActivateAbilityByKey(AbilityActivationKey::AbilityKey1);
}

void PlayerCharacter::ActivateAbility2()
{
	abilitiesComponent->ActivateAbilityByKey(AbilityActivationKey::AbilityKey2);
}

void PlayerCharacter::ActivateAbility3()
{
	abilitiesComponent->ActivateAbilityByKey(AbilityActivationKey::AbilityKey3);
}

void PlayerCharacter::ActivateAbility4()
{
	abilitiesComponent->ActivateAbilityByKey(AbilityActivationKey::AbilityKey4);
}

void PlayerCharacter::TakeDamage(float damageAmount)
{
	if (SceneApp::instance->GetCurrentGameTime() < invincibilityEndTime) return;

	float damageReductionMultiplier = 1 - (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Toughness) * 0.1);
	damageAmount *= damageReductionMultiplier;

	attributes->ApplyAttributeChange(HellwatchAttribute::Health, -damageAmount);
	ApplyInvincibilityForDuration(1.f);

	if (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) <= 0.f)
	{
		DisableUpdate();
		SceneApp::instance->SetGameState(GameState::GameEnd);
	}
}

void PlayerCharacter::ApplyInvincibilityForDuration(float invincibilityDuration)
{
	invincibilityEndTime = SceneApp::instance->GetCurrentGameTime() + invincibilityDuration;
}

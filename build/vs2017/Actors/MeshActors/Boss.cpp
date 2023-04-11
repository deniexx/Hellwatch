#include "Boss.h"

Boss::Boss() {

}

void Boss::Update(float deltaTime) {

}

void Boss::PostInit() {
	attributes = CreateComponent<AttributeComponent>();

	FAttribute health;
	health.attributeType = HellwatchAttribute::Health;
	health.maxAmount = 700.f;
	health.currentAmount = 700.f;
	health.bClampedToZero = true;
	attributes->AddAttribute(health);

	b2BodyDef bodyDef;
	bodyDef.position.Set(GetTranslation().x(), GetTranslation().z());
	bodyDef.type = b2_dynamicBody;
	bodyDef.enabled = true;
	bodyDef.linearDamping = 5.f;

	b2PolygonShape shape;
	shape.SetAsBox(5.f, 5.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;

	SetCollisionBody(SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, this));

	bossMovement = CreateComponent<CharacterMovementComponent>();
	bossMovement->Init(this);
	bossMovement->SetMaximumSpeed(30);
	bossMovement->SetAcceleration(150);
}

void Boss::Slam(float deltaTime) {
	gef::Vector4 bossPosition = GetTranslation();


}
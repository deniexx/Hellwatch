#include "Boss.h"

Boss::Boss() {

}

void Boss::Update(float deltaTime) 
{
	MeshActor::Update(deltaTime);

	if (PlayerCharacter* player = SceneApp::instance->GetPlayerCharacter())
	{
		gef::Vector4 playerPosition = player->GetTranslation();
		b2Vec2 playerDirection = b2Vec2(playerPosition.x(), playerPosition.z());
		gef::Vector4 enemyPosition = GetTranslation();
		b2Vec2 enemyDirection = b2Vec2(enemyPosition.x(), enemyPosition.z());
		b2Vec2 towardsPlayer = playerDirection - enemyDirection;
		towardsPlayer.Normalize();
		bossMovement->ApplyMovementForceInDirection(towardsPlayer);
	}
}

void Boss::PostInit() 
{
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
	shape.SetAsBox(3.f, 3.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;

	SetCollisionBody(SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, this));

	bossMovement = CreateComponent<CharacterMovementComponent>();
	bossMovement->Init(this);
	bossMovement->SetMaximumSpeed(50);
	bossMovement->SetAcceleration(60);

	SetMesh(SceneApp::instance->RequestMeshByName("Warrok"));
	SetScale(gef::Vector4(3.f, 3.f, 3.f));

	gef::Material mat;
	mat.set_texture(SceneApp::instance->RequestTextureByName("Boss"));
	SetMaterial(mat);

	damageAmount = 80;
}

//void Boss::Slam(float deltaTime) {
//	gef::Vector4 bossPosition = GetTranslation();
//
//
//}
//
//void Boss::Jump(float deltaTime) {
//	if (PlayerCharacter* player = SceneApp::instance->GetPlayerCharacter()) {
//		playerPosition = player->GetTranslation();
//		bossPosition = GetTranslation();
//		gef::Vector4 midpoint = (playerPosition + bossPosition) / 2;
//		midpoint.set_y(midpoint.y() + 15);
//		alpha = 0;
//		isJumping = true;
//	}
//}
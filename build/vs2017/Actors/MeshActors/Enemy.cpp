#include "Enemy.h"

void Enemy::TakeDamage(float damageAmount)
{
	if (attributes)
		attributes->ApplyAttributeChange(HellwatchAttribute::Health, -damageAmount);
}

void Enemy::PostInit()
{
	attributes = CreateComponent<AttributeComponent>();

	FAttribute health;
	health.attributeType = HellwatchAttribute::Health;
	health.maxAmount = 100.f;
	health.currentAmount = 100.f;
	health.bClampedToZero = true;
	attributes->AddAttribute(health);

	b2BodyDef bodyDef;
	bodyDef.position.Set(GetTranslation().x(), GetTranslation().z());
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

	enemyMovement = CreateComponent<CharacterMovementComponent>();
	enemyMovement->Init(this);
}

void Enemy::Update(float deltaTime) 
{
	Super::Update(deltaTime);

	if (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) <= 0.f)
	{
		DisableUpdate();
		MarkForDelete();
		return;
	}

	if (PlayerCharacter* player = SceneApp::instance->GetPlayerCharacter())
	{
		gef::Vector4 playerPosition = player->GetTranslation();
		b2Vec2 playerDirection = b2Vec2(playerPosition.x(), playerPosition.z());
		gef::Vector4 enemyPosition = GetTranslation();
		b2Vec2 enemyDirection = b2Vec2(enemyPosition.x(), enemyPosition.z());
		b2Vec2 towardsPlayer = playerDirection - enemyDirection;
		towardsPlayer.Normalize();
		enemyMovement->ApplyMovementForceInDirection(towardsPlayer);
	}
}
#include "RangedEnemy.h"

void RangedEnemy::TakeDamage(float damageAmount)
{
	if (attributes)
		attributes->ApplyAttributeChange(HellwatchAttribute::Health, -damageAmount);

	if (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) <= 0.f)
	{
		DisableUpdate();
		MarkForDelete();
	}
}

void RangedEnemy::PostInit()
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
	shape.SetAsBox(1.f, 1.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;

	SetCollisionBody(SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, this));

	enemyMovement = CreateComponent<CharacterMovementComponent>();
	enemyMovement->Init(this);
}


void RangedEnemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	PlayerCharacter* player = SceneApp::instance->GetPlayerCharacter();
	gef::Vector4 playerPosition = player->GetTranslation();
	b2Vec2 playerDirection = b2Vec2(playerPosition.x(), playerPosition.z());
	gef::Vector4 enemyPosition = GetTranslation();
	b2Vec2 enemyDirection = b2Vec2(enemyPosition.x(), enemyPosition.z());
	b2Vec2 towardsPlayer = playerDirection - enemyDirection;

	if (towardsPlayer.Length() >= 10.f) {
		if (player)
		{
			towardsPlayer.Normalize();
			enemyMovement->ApplyMovementForceInDirection(towardsPlayer);
		}
	}
	else if (towardsPlayer.Length() <= 7.0f) {
		towardsPlayer.Normalize();
		enemyMovement->ApplyMovementForceInDirection(-towardsPlayer);
	}

}

void RangedEnemy::OnCollision(b2Body* otherBody)
{
	if (attackTime + attackCooldown < SceneApp::instance->GetCurrentGameTime()) {
		if (otherBody)
		{
			PlayerCharacter* player = (PlayerCharacter*)otherBody->GetUserData().pointer;
			if (player)
			{
				player->TakeDamage(damageAmount);
				attackTime = SceneApp::instance->GetCurrentGameTime();
			}
		}
	}
}
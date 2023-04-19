#include "Enemy.h"

Enemy::Enemy()
{
	ID = ENEMY_ID;
}

void Enemy::SetClass(EnemyClass::Type type)
{
	switch (type)
	{
	case EnemyClass::HEAVY:
	{
		playerMoneyToAdd *= 1.5;
		enemyMovement->SetAcceleration(enemyMovement->GetAcceleration() * 0.5f);
		attributes->ApplyAttributeMultipliyer(HellwatchAttribute::Health, 1.5f);
		damageAmount = damageAmount * 1.5f;
	}
		break;
	case EnemyClass::NORMAL:
	{
		//enemyMovement->SetAcceleration(enemyMovement->GetAcceleration());
		//attributes->SetAttributeCurrentAndMaxValue(HellwatchAttribute::Health, 100.f);
		//damageAmount = 50;
	}
		break;
	case EnemyClass::LIGHT:
	{
		playerMoneyToAdd *= 0.5;
		enemyMovement->SetAcceleration(enemyMovement->GetAcceleration() * 1.5f);
		attributes->ApplyAttributeMultipliyer(HellwatchAttribute::Health, 0.5f);
		damageAmount = damageAmount * 0.5f;
	}
		break;
	}
}

void Enemy::TakeDamage(float damageAmount)
{
	if (attributes)
	{
		attributes->ApplyAttributeChange(HellwatchAttribute::Health, -damageAmount);

		if (attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) <= 0.f)
		{
			IncreasePlayerMoney();
			DisableUpdate();
			MarkForDelete();
		}
	}
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

	SetMesh(SceneApp::instance->RequestMeshByName("MutantMesh"));
	gef::Material mat;
	mat.set_texture(SceneApp::instance->RequestTextureByName("MeleeEnemy"));
	SetMaterial(mat);

	damageAmount = 50;
}

void Enemy::IncreasePlayerMoney()
{
	SceneApp::instance->IncreasePlayerMoney(playerMoneyToAdd);
}

void Enemy::Update(float deltaTime) 
{
	Super::Update(deltaTime);

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

void Enemy::OnCollision(b2Body* otherBody)
{
	if (attackTime + attackCooldown < SceneApp::instance->GetCurrentGameTime()) {
		if (otherBody)
		{
			MeshActor* actor = (MeshActor*)otherBody->GetUserData().pointer;
			if (actor && actor->ID == PLAYER_ID)
			{
				actor->TakeDamage(damageAmount);
				attackTime = SceneApp::instance->GetCurrentGameTime();
			}
		}
	}
}

void Enemy::ScaleWithWave(int wave)
{
	float multipliyer = (wave * 0.1) + 1;
	attributes->ApplyAttributeMultipliyer(HellwatchAttribute::Health, multipliyer);
	damageAmount *= multipliyer;
	playerMoneyToAdd *= multipliyer;
}

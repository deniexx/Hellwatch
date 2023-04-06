#pragma once
#include "Enemy.h"
class RangedEnemy : public Enemy
{
    GENERATED_BODY(MeshActor, RangedEnemy)

public:

    RangedEnemy();

    virtual void TakeDamage(float damageAmount);
    float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
    virtual void Update(float deltaTime) override;
    virtual void Shoot(float deltaTime);

    float damageAmount;
    float attackTime;
    float attackCooldown = 2;

private:
    AttributeComponent* attributes;
    virtual void PostInit() override;
    CharacterMovementComponent* enemyMovement;

};


#pragma once
#include "Enemy.h"
class RangedEnemy :
    public Enemy
{
    GENERATED_BODY(MeshActor, RangedEnemy)
public:
    virtual void TakeDamage(float damageAmount) override;
    float GetHealth() { return attributes ? attributes->GetCurrentAttributeValueByType(HellwatchAttribute::Health) : 0.f; }
    virtual void Update(float deltaTime) override;
    virtual void OnCollision(b2Body* otherBody) override;

    float damageAmount;
    float attackTime;
    float attackCooldown = 2;

private:
    AttributeComponent* attributes;
    virtual void PostInit() override;
    CharacterMovementComponent* enemyMovement;

};


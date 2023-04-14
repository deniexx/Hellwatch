#pragma once
#include "Enemy.h"
class RangedEnemy : public Enemy
{
    GENERATED_BODY(MeshActor, RangedEnemy)

public:

    RangedEnemy();

    virtual void Update(float deltaTime) override;
    virtual void Shoot(float deltaTime);

protected:

    virtual void PostInit() override;
};


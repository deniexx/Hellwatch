#pragma once

#include "Enemy.h"

/// <summary>
/// Ranged enemy implementation, tries to keep distance and shoots from afar
/// </summary>
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


#include "DamageOnCollisionActor.h"

void DamageOnCollisionActor::OnCollision(b2Body* otherBody)
{
	if (otherBody)
	{
		MeshActor* actor = (MeshActor*)otherBody->GetUserData().pointer;
		if (actor)
		{
			bool bApplyDamage = false;
			if (applyDamageOn == ApplyDamageOn::PlayerOnly)
			{
				bApplyDamage = actor->ID == PLAYER_ID;
			}
			else if (applyDamageOn == ApplyDamageOn::EnemiesOnly)
			{
				bApplyDamage = actor->ID == ENEMY_ID;
			}
			else
			{
				bApplyDamage = (actor->ID == ENEMY_ID) || (actor->ID == PLAYER_ID);
			}
			
			if (bApplyDamage)
			{
				actor->TakeDamage(damageAmount);
				MarkForDelete();
				return;
			}

			if (actor->ID == ENVIRONMENT_ID)
				MarkForDelete();
		}
	}
}

void DamageOnCollisionActor::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (bSpinning)
	{
		gef::Vector4 actorRot = GetRotation();
		actorRot.set_y(actorRot.y() + deltaTime * 100.f);
		SetRotation(actorRot);
	}
}

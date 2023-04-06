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
			}
		}
	}
}

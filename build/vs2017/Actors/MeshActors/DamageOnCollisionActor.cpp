#include "DamageOnCollisionActor.h"

void DamageOnCollisionActor::OnCollision(b2Body* otherBody)
{
	if (otherBody)
	{
		MeshActor* actor = (MeshActor*)otherBody->GetUserData().pointer;
		if (actor)
		{
			actor->TakeDamage(damageAmount);
			MarkForDelete();
		}
	}
}

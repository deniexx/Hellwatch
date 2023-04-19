#include "MeteorActor.h"

void MeteorActor::Update(float deltaTime)
{
	if (lerpAlpha < 1.f)
	{
		lerpAlpha += deltaTime * 0.5f;
		SetTranslation(Lerp(startPosition, targetPosition, lerpAlpha));
	}
	else
	{
		SceneApp::instance->ApplyRadialDamage(damageAmount, targetPosition, 7, 15);
		MarkForDelete();
	}

}

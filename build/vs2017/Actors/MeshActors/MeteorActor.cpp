#include "MeteorActor.h"
#include "GameFramework/ParticleManager.h"

void MeteorActor::Update(float deltaTime)
{
	if (lerpAlpha < 1.f)
	{
		lerpAlpha += deltaTime * 0.5f;
		SetTranslation(Lerp(startPosition, targetPosition, lerpAlpha));
	}
	else
	{
		SceneApp::instance->PlaySample("MeteorImpact");
		SceneApp::instance->ApplyRadialDamage(damageAmount, targetPosition, 7, 15);
		FCameraShake cameraShake;
		cameraShake.duration = 0.75f;
		cameraShake.maxOffset = gef::Vector4(1.f, 1.f, 1.f);
		cameraShake.minOffset = gef::Vector4(-1.f, -1.f, -1.f);
		cameraShake.intensity = 30.f;
		SceneApp::instance->ApplyCameraShake(cameraShake);
		ParticleManager::instance->PlayParticlesAtLocation(HellwatchParticle::Explosion, targetPosition);
		MarkForDelete();
	}
}

#include "PiercingStrike.h"

#include "scene_app.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "maths/math_utils.h"
#include "Actors/MeshActors/DamageOnCollisionActor.h"

void PiercingStrike::PostInit()
{
	SetAbilityName("Piercing Strike");
	SetAbilityCostType(HellwatchAttribute::Mana);
	SetAbilityCostAmount(15.f);
	SetAbilityCooldown(1.25f);
	SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);
	SetDamageAmount(15.f);
}

void PiercingStrike::Begin()
{
	Super::Begin();

	gef::Vector4 mousePos = ProjectScreenToWorldSpace(SceneApp::instance->GetLastTouchPosition());
	gef::Vector4 translation = GetOwner()->GetOwner()->GetTranslation();
	gef::Vector4 target = mousePos - translation;
	target.Normalise();

	translation.set_y(1.f);
	gef::Mesh* mesh = const_cast<gef::Mesh*>(SceneApp::instance->GetPrimitiveBuilder()->GetDefaultCubeMesh());
	DamageOnCollisionActor* actor = SceneApp::instance->SpawnMeshActor<DamageOnCollisionActor>(mesh, translation);
	actor->SetDamageAmount(GetDamageAmount());
	actor->SetScale(gef::Vector4(0.75f, 0.25f, 0.75f));
	actor->SetSpinning(true);
	actor->SetIsPiercing(true);

	gef::Material mat;
	mat.set_colour(0xFF05E2FC);
	mat.set_texture(SceneApp::instance->RequestTextureByName("IceBolt"));
	actor->SetMaterial(mat);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(translation.x(), translation.z());
	bodyDef.enabled = true;

	b2PolygonShape shape;
	shape.SetAsBox(0.4f, 0.4f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.75f;
	fixtureDef.isSensor = true;

	b2Body* body = SceneApp::instance->CreateCollisionBody(bodyDef, fixtureDef, actor);
	actor->SetCollisionBody(body);
	b2Vec2 forceDir = b2Vec2(target.x(), target.z());
	forceDir *= 1500.f;

	actor->GetCollisionBody()->ApplyForceToCenter(forceDir, true);

	GetOwnerAttributes()->ApplyAttributeChange(abilityCostType, -abilityCostAmount);
}

void PiercingStrike::End()
{
	Super::End();
}

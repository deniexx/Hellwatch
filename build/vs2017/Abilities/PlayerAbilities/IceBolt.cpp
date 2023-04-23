#include "IceBolt.h"
#include "scene_app.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "maths/math_utils.h"
#include "Actors/MeshActors/DamageOnCollisionActor.h"

IceBolt::IceBolt()
	: Super()
{
}

void IceBolt::PostInit()
{
	SetAbilityName("IceBolt");
	SetAbilityCostType(HellwatchAttribute::Mana);
	SetAbilityCostAmount(10.f);
	SetAbilityCooldown(0.75f);
	SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);
	SetDamageAmount(20.f);
}

void IceBolt::Begin()
{
	Super::Begin();

	gef::Vector4 mousePos = ProjectScreenToWorldSpace(SceneApp::instance->GetLastTouchPosition());
	gef::Vector4 translation = GetOwner()->GetOwner()->GetTranslation();
	gef::Vector4 target = mousePos - translation;
	target.Normalise();

	translation.set_y(1.f);
	gef::Mesh* mesh = SceneApp::instance->RequestMeshByName("IceBolt");
	DamageOnCollisionActor* actor = SceneApp::instance->SpawnMeshActor<DamageOnCollisionActor>(mesh, translation);
	actor->SetDamageAmount(GetDamageAmount());
	actor->SetScale(gef::Vector4(0.2f, 0.2f, 0.2f));
	actor->SetSpinning(true);

	gef::Material mat;
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

	SceneApp::instance->PlaySample("Ice1");

	GetOwnerAttributes()->ApplyAttributeChange(abilityCostType, -abilityCostAmount);
}

void IceBolt::End()
{
}

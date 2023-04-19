#include "Meteor.h"

#include "primitive_builder.h"
#include "ActorComponents/AbilitiesComponent.h"
#include "Actors/MeshActors/MeteorActor.h"

Meteor::Meteor()
	: Super()
{
}

void Meteor::PostInit()
{
	SetAbilityName("Meteor");
	SetAbilityCostType(HellwatchAttribute::Mana);
	SetAbilityCostAmount(30.f);
	SetAbilityCooldown(10.f);
	SetCooldownStartPolicy(CooldownStartPolicy::OnAbilityStart);
	SetDamageAmount(150.f);
}

void Meteor::Begin()
{
	Super::Begin();

	gef::Vector4 mousePos = ProjectScreenToWorldSpace(SceneApp::instance->GetLastTouchPosition());
	gef::Vector4 translation = GetOwner()->GetOwner()->GetTranslation();
	translation.set_y(15.f);

	gef::Mesh* mesh = const_cast<gef::Mesh*>(SceneApp::instance->GetPrimitiveBuilder()->GetDefaultSphereMesh());
	//gef::Mesh* mesh = SceneApp::instance->RequestMeshByName("Sphere");
	MeteorActor* actor = SceneApp::instance->SpawnMeshActor<MeteorActor>(mesh, translation);
	actor->SetDamageAmount(GetDamageAmount());
	actor->SetScale(gef::Vector4(4.f, 4.f, 4.f));
	actor->SetSpinning(true);

	gef::Material mat;
	mat.set_texture(SceneApp::instance->RequestTextureByName("Meteor"));
	actor->SetMaterial(mat);

	actor->startPosition = translation;
	actor->targetPosition = mousePos;

	GetOwnerAttributes()->ApplyAttributeChange(abilityCostType, -abilityCostAmount);
}

void Meteor::End()
{
	Super::End();
}

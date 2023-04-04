#include "MeshActor.h"

#include "scene_app.h"
#include "graphics/renderer_3d.h"
#include "ActorComponents/ActorComponent.h"

MeshActor::MeshActor()
	: Super()
{
	mesh = nullptr;
	material.set_colour(0xFFFFFFFF);
	bOverrideMaterial = false;
	material.set_texture(nullptr);
	material.set_colour(0xFFFFFFFF);
}

MeshActor::MeshActor(gef::Mesh* inMesh)
	: Super()
{
	mesh = inMesh;
}

void MeshActor::Render()
{
	if (SceneApp::instance)
	{
		if (SceneApp::instance->GetRenderer3D())
		{
			BuildTransform();

			SceneApp::instance->GetRenderer3D()->set_override_material(&material);

			SceneApp::instance->GetRenderer3D()->DrawMesh(*GetMesh(), GetTransform());
		}
	}
}

void MeshActor::Update(float deltaTime)
{
	b2Body* body = GetCollisionBody();

	if (body != nullptr)
	{
		b2Vec2 translation = body->GetPosition();
		SetTranslation(gef::Vector4(translation.x, 0, translation.y));
	}

	for (auto component : components)
	{
		component->UpdateComponent(deltaTime);
	}
}

void MeshActor::SetRotation(const gef::Vector4& inRotation)
{
	gef::Vector4 tempRotation = gef::Vector4(inRotation.x() * 360, inRotation.y() * 360, inRotation.z() * 360);
	Super::SetRotation(tempRotation);
}

#include "MeshActor.h"

#include "scene_app.h"
#include "graphics/renderer_3d.h"

MeshActor::MeshActor()
	: Super()
{
	mesh = nullptr;
	material.set_colour(0xFFFFFFFF);
	bOverrideMaterial = false;
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

			if (bOverrideMaterial)
				SceneApp::instance->GetRenderer3D()->set_override_material(&material);

			SceneApp::instance->GetRenderer3D()->DrawMesh(*GetMesh(), GetTransform());
		}
	}
}

#include "MeshActor.h"

#include "scene_app.h"
#include "graphics/renderer_3d.h"

MeshActor::MeshActor()
	: Super()
{
	mesh = nullptr;
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
			SceneApp::instance->GetRenderer3D()->DrawMesh(*GetMesh(), GetTransform());
		}
	}
}

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
	if (GetScene())
	{
		if (GetScene()->GetRenderer3D())
		{
			GetScene()->GetRenderer3D()->DrawMesh(*GetMesh(), GetTransform());
		}
	}
}

#include "WorldObject.h"

#include "graphics/scene.h"
#include "scene_app.h"

WorldObject::WorldObject()
{
	transformMat.SetIdentity();
	SetTranslation(gef::Vector4::kZero);
	SetRotation(gef::Quaternion::kIdentity);
	SetScale(gef::Vector4::kOne);
}

void WorldObject::Init(SceneApp* scene, WorldObject* owner)
{
	owningScene = scene;
	owningObject = owner;

	PostInit();
}

void WorldObject::PostInit()
{
}

void WorldObject::Update(float deltaTime)
{
}

void WorldObject::Render()
{
}

void WorldObject::SetTranslation(const gef::Vector4& inTranslation)
{
	transformMat.SetTranslation(inTranslation);
}

void WorldObject::SetRotation(const gef::Quaternion& inRotation)
{
	gef::Matrix44 rotationMatrix;
	rotationMatrix.SetIdentity();
	rotationMatrix.Rotation(rotationMatrix);
	rotationVec = inRotation;

	transformMat = transformMat * rotationMatrix;
}

void WorldObject::SetRotation(const float inRotation)
{
	// To be overriden for sprites
}

void WorldObject::SetScale(const gef::Vector4& inScale)
{
	gef::Matrix44 scale;
	scale.SetIdentity();
	scale.Scale(inScale);

	transformMat = transformMat * scale;
}

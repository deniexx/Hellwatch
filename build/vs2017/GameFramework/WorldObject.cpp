#include "WorldObject.h"

#include "graphics/scene.h"
#include "scene_app.h"

WorldObject::WorldObject()
{
	transformMat.SetIdentity();
	SetTranslation(gef::Vector4::kZero);
	SetRotation(gef::Vector4::kZero);
	SetScale(gef::Vector4::kOne);
}

void WorldObject::Init(WorldObject* owner)
{
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
	translation = inTranslation;
}

void WorldObject::SetRotation(const gef::Vector4& inRotation)
{
	rotationVec = inRotation;
}

void WorldObject::SetRotation(const float inRotation)
{
	// To be overriden for sprites
}

void WorldObject::SetScale(const gef::Vector4& inScale)
{
	scale = inScale;
}

void WorldObject::BuildTransform()
{
	gef::Matrix44 scale_mat, rotateX_mat, rotateY_mat, rotateZ_mat, translate_mat;
	scale_mat.SetIdentity();
	scale_mat.Scale(scale);

	rotateX_mat.RotationX(rotationVec.x());
	rotateY_mat.RotationY(rotationVec.y());
	rotateZ_mat.RotationZ(rotationVec.z());

	translate_mat.SetIdentity();
	translate_mat.SetTranslation(translation);

	transformMat = scale_mat * rotateX_mat * rotateY_mat * rotateZ_mat * translate_mat;
}

void WorldObject::OnCollision(b2Body* OtherBody)
{
}

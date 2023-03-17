#include "Character.h"
#include <primitive_builder.h>
#include <graphics/renderer_3d.h>

void Character::Init(const PrimitiveBuilder& builder)
{
	set_mesh(builder.GetDefaultCubeMesh());
}

void Character::Render(gef::Renderer3D* renderer)
{
	renderer->DrawMesh(*this);
}

void Character::SetTranslation(gef::Vector4 newTranslation)
{
	transform_.SetTranslation(newTranslation);
}

const gef::Vector4 Character::GetTranslation()
{
	return transform_.GetTranslation();
}

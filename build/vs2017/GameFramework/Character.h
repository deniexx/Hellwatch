#pragma once

#include "graphics/mesh_instance.h"

class PrimitiveBuilder;

namespace gef
{
	class Renderer3D;
}

class Character : public gef::MeshInstance
{
public:

	void Init(const PrimitiveBuilder& builder);
	void Render(gef::Renderer3D* renderer);

private:
	gef::Matrix44 translation;

public:

	void SetTranslation(gef::Vector4 newTranslation);
	const gef::Vector4 GetTranslation();
};
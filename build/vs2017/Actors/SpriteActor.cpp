#include "SpriteActor.h"
#include "graphics/sprite_renderer.h"
#include <scene_app.h>

SpriteActor::SpriteActor()
	: Super()
{
	sprite = nullptr;
}

SpriteActor::SpriteActor(float width, float height, gef::Texture* texture, UInt32 colour)
	: Super()
{
	sprite = new gef::Sprite();
	sprite->set_width(width);
	sprite->set_height(height);
	
	if (texture)
		sprite->set_texture(texture);

	sprite->set_colour(colour);
}

void SpriteActor::SetTranslation(const gef::Vector4& inTranslation)
{
	sprite->set_position(inTranslation);
}

void SpriteActor::SetRotation(const float rotation)
{
	sprite->set_rotation(rotation);
}

void SpriteActor::SetScale(const gef::Vector4& inScale)
{
	sprite->set_width(inScale.x());
	sprite->set_height(inScale.y());
}

void SpriteActor::SetTexture(gef::Texture* texture)
{
	sprite->set_texture(texture);
}

void SpriteActor::SetSprite(gef::Sprite* inSprite)
{
	sprite = inSprite;
}

void SpriteActor::Render()
{
	if (SceneApp::instance)
	{
		if (SceneApp::instance->GetSpriteRenderer())
		{
			SceneApp::instance->GetSpriteRenderer()->DrawSprite(*sprite);
		}
	}
}

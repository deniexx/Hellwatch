#pragma once

#include "WorldObject.h"

#include "graphics/sprite.h"
#include "graphics/texture.h"

class SpriteActor : public WorldObject
{
	GENERATED_BODY(WorldObject, SpriteActor)

public:

	/// <summary>
	/// Default constructor
	/// </summary>
	SpriteActor();

	/// <summary>
	/// Constructor that sets up the sprite
	/// </summary>
	/// <param name="width">Width of the sprite</param>
	/// <param name="height">Height of the sprite</param>
	/// <param name="texture">Texture of the sprite</param>
	/// <param name="colour">Colour of the sprite</param>
	SpriteActor(float width, float height, gef::Texture* texture = nullptr, UInt32 colour = 0xFFFFFFFF);

private:

	gef::Sprite* sprite;

public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	virtual void SetTranslation(const gef::Vector4& inTranslation) override;
	virtual void SetRotation(const float rotation) override;
	virtual void SetScale(const gef::Vector4& inScale);
	void SetTexture(gef::Texture* texture);
	void SetSprite(gef::Sprite* inSprite);


	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const gef::Sprite* GetSprite() const { return sprite; }
	__forceinline const virtual float GetRotationF() const override { return sprite->rotation(); }
	__forceinline const virtual gef::Vector4& GetTranslation() const { return sprite->position(); }
	__forceinline const virtual gef::Vector4& GetScale() const { return gef::Vector4(sprite->width(), sprite->height(), 0); }
};


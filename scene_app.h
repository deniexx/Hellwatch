#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include "graphics/scene.h"
#include "box2d/box2d.h"
#include <vector>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
	class Sprite;
}

class MeshActor;
class SpriteActor;
class WorldObject;

class SceneApp : public gef::Application
{
public:

	static inline SceneApp* SceneApp::instance;

	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

	b2Body* CreateCollisionBody(b2BodyDef bodyDef, b2FixtureDef fixtureDef, WorldObject* owningObject);

	MeshActor* SpawnMeshActor(gef::Mesh* mesh = nullptr, gef::Vector4 translation = gef::Vector4::kZero, gef::Vector4 rotation = gef::Vector4::kZero, gef::Vector4 scale = gef::Vector4::kZero);

	SpriteActor* SpawnSpriteActor(gef::Sprite* sprite = nullptr, gef::Vector2 position = gef::Vector2::kZero, float rotation = 0.f);

private:

	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void HandleCollision();
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;

	PrimitiveBuilder* primitive_builder_;

	std::vector<SpriteActor*> spriteActors;
	std::vector<MeshActor*> meshActors;
	gef::Scene* scene_assets_;

	float fps_;

	/************************************************************************/
	/*                               BOX2D                                  */
	/************************************************************************/
	b2World* b2dWorld;
	b2Vec2 gravity;


public:

	__forceinline gef::Renderer3D* GetRenderer3D() const { return renderer_3d_; }
	__forceinline gef::SpriteRenderer* GetSpriteRenderer() const { return sprite_renderer_; }
	__forceinline b2World* GetBox2DWorld() const { return b2dWorld; }
};

#endif // _SCENE_APP_H

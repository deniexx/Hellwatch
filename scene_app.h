#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include "graphics/scene.h"
#include "box2d/box2d.h"
#include "system/platform.h"
#include <vector>
#include "obj_mesh_loader.h"
#include <future>
#include "graphics/font.h"

namespace GameState
{
	enum Type
	{
		Loading,
		MainMenu,
		GameLoop,
		PauseMenu,
		Shop
	};
}

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

class PlayerController;
class MeshActor;
class SpriteActor;
class WorldObject;
class PlayerCharacter;
class EnemyDummy;
class Enemy;
class RangedEnemy;
class WaveManager;
class MainMenu;
class ShopMenu;

typedef std::map<std::string, gef::Texture*> TextureMap;

class SceneApp : public gef::Application
{
public:

	static inline SceneApp* SceneApp::instance;

	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

	/// <summary>
	/// Creates a collision body for a specified object, object can be null if using for things like environment or invisible walls
	/// @NOTE: The owning object will be used to call OnCollision function
	/// </summary>
	b2Body* CreateCollisionBody(b2BodyDef bodyDef, b2FixtureDef fixtureDef, WorldObject* owningObject);

private:

	void InitFont();
	void LoadAssets();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void HandleCollision();
	void CheckMarkedForDeletion();
	void BuildToLoadData();
	void InitGameLoop();
	void InitMainMenu();
	void InitShop();

	void UpdateLoading(float frame_time);
	void UpdateMainMenu(float frame_time);
	void UpdateGameLoop(float frame_time);
	void UpdatePauseMenu(float frame_time);
	void UpdateShop();

	void RenderLoading();
	void RenderMainMenu();
	void RenderGameLoop();
	void RenderPauseMenu();
	void RenderShop();


	/*******************************************************
	*					GAME STATE                         *
	*******************************************************/
	GameState::Type gameState;
	bool bGameLoopInitted = false;

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;

	PrimitiveBuilder* primitive_builder_;
	MeshMap meshes;
	TextureMap textures;

	std::future<GameState::Type> loadFuture;

	WaveManager* waveManager;
	MainMenu* mainMenu;
	ShopMenu* shopMenu;


	/************************************************************************/
	/*                              REQUIRED                                */
	/************************************************************************/
	gef::Sprite* loadingSprite;
	PlayerCharacter* playerCharacter;
	std::vector<MeshActor*> meshActors;
	std::vector<SpriteActor*> spriteActors;
	gef::Scene* scene_assets_;
	std::vector<std::string> meshesToLoad;
	std::map<std::string, std::string> texturesToLoad;

	gef::Vector4 cameraEye = gef::Vector4(0.0f, 20.0f, 0.0f);
	gef::Vector4 cameraLookAt = gef::Vector4(0.0f, -1.0f, 0.001f);
	gef::Vector4 cameraUp = gef::Vector4(0.0f, 1.0f, 0.0f);

	float fps_;
	float currentGameTime;
	float lastDeltaTime;

	/************************************************************************/
	/*                               BOX2D                                  */
	/************************************************************************/
	b2World* b2dWorld;
	b2Vec2 gravity;

public:

	__forceinline gef::Renderer3D* GetRenderer3D() const { return renderer_3d_; }
	__forceinline gef::SpriteRenderer* GetSpriteRenderer() const { return sprite_renderer_; }
	__forceinline b2World* GetBox2DWorld() const { return b2dWorld; }
	__forceinline float GetViewportWidth() const { return platform_.width(); }
	__forceinline float GetViewportHeight() const { return platform_.height(); }
	__forceinline PlayerCharacter* GetPlayerCharacter() const { return playerCharacter; }
	__forceinline PrimitiveBuilder* GetPrimitiveBuilder() const { return primitive_builder_; }
	__forceinline gef::Vector4 GetCameraEye() const { return cameraEye; }
	__forceinline gef::Vector4 GetCameraLookAt() const { return cameraLookAt; }
	__forceinline float GetCurrentGameTime() const { return currentGameTime; }
	__forceinline float GetLastDeltaTime() const { return lastDeltaTime; }
	gef::Mesh* RequestMeshByName(std::string meshName);
	gef::Texture* RequestTextureByName(std::string textureName);
	static const gef::Vector2 GetLastTouchPosition();

	// Game State
	void SetGameState(GameState::Type newState);
	GameState::Type GetGameState() const { return gameState; }



	/// <summary>
	/// Spawns a MeshActor into the scene and gets it ready for rendering and updating
	/// </summary>
	template <typename MeshActorType = MeshActor>
	MeshActorType* SpawnMeshActor(gef::Mesh* mesh = nullptr, gef::Vector4 translation = gef::Vector4::kZero, gef::Vector4 rotation = gef::Vector4::kZero, gef::Vector4 scale = gef::Vector4::kOne, WorldObject* owner = nullptr)
	{
		MeshActorType* meshActor = new MeshActorType();
		meshActor->SetMesh(mesh);
		meshActor->SetRotation(rotation);
		meshActor->SetScale(scale);
		meshActor->Init(owner);
		meshActor->SetTranslation(translation);
		meshActors.push_back(meshActor);

		return meshActor;
	}

	/// <summary>
	/// Spawns a SpriteActor into the scene and gets it ready for rendering and updating
	/// </summary>
	template<typename SpriteActorType = SpriteActor>
	SpriteActorType* SpawnSpriteActor(gef::Sprite* sprite = nullptr, gef::Vector2 position = gef::Vector2::kZero, float rotation = 0.f, WorldObject* owner = nullptr)
	{
		SpriteActorType* spriteActor = new SpriteActorType();
		spriteActor->SetSprite(sprite);
		spriteActor->SetTranslation(gef::Vector4(position.x, position.y, 0.f));
		spriteActor->SetRotation(rotation);
		spriteActor->Init(owner);
		spriteActors.push_back(spriteActor);

		return spriteActor;
	}
};

#endif // _SCENE_APP_H

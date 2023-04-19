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

/* Forward declarations */
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
class PauseMenu;

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

	/************************************************************************/
	/*                         CALLED ONCE                                  */
	/************************************************************************/
	void InitFont();
	void LoadAssets();
	void CleanUpFont();
	void SetupLights();
	void BuildToLoadData();
	void InitGameLoop();
	void InitMainMenu();
	void InitShop();
	void InitPauseMenu();

	/************************************************************************/
	/*                             UPDATES                                  */
	/************************************************************************/
	void UpdateLoading(float frame_time);
	void UpdateMainMenu(float frame_time);
	void UpdateGameLoop(float frame_time);
	void UpdatePauseMenu(float frame_time);
	void UpdateShop();

	void HandleCollision();
	void CheckMarkedForDeletion();


	/************************************************************************/
	/*                           RENDERING                                  */
	/************************************************************************/
	void DrawHUD();

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
	bool bCommingFromMainMenu = false;

	/*******************************************************
	*				   ASSET LOADING                       *
	*******************************************************/
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	PrimitiveBuilder* primitive_builder_;
	MeshMap meshes;
	TextureMap textures;

	/*******************************************************
	*					 RENDERING                         *
	*******************************************************/
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;

	/* This is used to asynchronously load assets */
	std::future<GameState::Type> loadFuture;

	/* Menu classes and wave manager */
	WaveManager* waveManager;
	MainMenu* mainMenu;
	ShopMenu* shopMenu;
	PauseMenu* pauseMenu;

	/* We store this here for ease of access */
	uint32_t playerMoney = 0;

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

	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
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
	gef::Font* GetFont() { return font_; }
	static const gef::Vector2 GetLastTouchPosition();

	/* Game State */
	void SetGameState(GameState::Type newState);
	GameState::Type GetGameState() const { return gameState; }

	/* Player money */
	const uint32_t GetPlayerMoney() { return playerMoney; }
	void IncreasePlayerMoney(uint32_t increaseAmount) { playerMoney += increaseAmount; }
	void ApplyCostToPlayerMoney(uint32_t cost) { playerMoney -= cost; }


	/* Damage */
	void ApplyRadialDamage(float damageAmount, gef::Vector4 origin, float innerRadius, float outerRadius);

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

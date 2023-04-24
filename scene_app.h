#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include "graphics/scene.h"
#include "box2d/box2d.h"
#include "system/platform.h"
#include "obj_mesh_loader.h"
#include "graphics/font.h"

#include <vector>
#include <future>
#include <random>

namespace GameState
{
	enum Type
	{
		Loading,
		MainMenu,
		GameLoop,
		PauseMenu,
		Shop,
		GameEnd
	};
}

struct FCameraShake
{
	gef::Vector4 maxOffset = gef::Vector4::kZero;
	gef::Vector4 minOffset = gef::Vector4::kZero;

	float lerpAlpha = 0.f;
	float duration = 1.f;
	float intensity = 1.f;
	float cameraShakeStartTime = 0.f;

	bool bMovingForward = true;
};

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
	class Sprite;
	class AudioManager;
}

/* Forward declarations */
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
class GameEndMenu;
class PlayerController;
class ParticleManager;

typedef std::map<std::string, gef::Texture*> TextureMap;
typedef std::map<std::string, int> SoundMap;

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
	void CleanUpFont();
	void SetupLights();
	void InitGameLoop();
	void InitMainMenu();
	void InitShop();
	void InitPauseMenu();
	void InitGameEndMenu();

	/************************************************************************/
	/*                             UPDATES                                  */
	/************************************************************************/
	void UpdateLoading(float frame_time);
	void UpdateMainMenu(float frame_time);
	void UpdateGameLoop(float frame_time);
	void UpdatePauseMenu(float frame_time);
	void UpdateShop();
	void UpdateGameEnd();

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
	void RenderGameEnd();

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

	void LoadAssets();
	void BuildToLoadData();

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
	GameEndMenu* gameEndMenu;

	/* We store this here for ease of access */
	uint32_t playerMoney = 0;

	/* Utility function to find sample name */
	int FindSampleFromName(std::string sampleName);

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
	std::map<std::string, std::string> soundsToLoad;

	gef::Sprite* pointerSprite;
	gef::Vector2 pointerLocation = gef::Vector2(0, 0);

	gef::Vector4 cameraEye = gef::Vector4(0.0f, 20.0f, 0.0f);
	gef::Vector4 cameraLookAt = gef::Vector4(0.0f, -1.0f, 0.001f);
	gef::Vector4 cameraUp = gef::Vector4(0.0f, 1.0f, 0.0f);

	FCameraShake cameraShake = FCameraShake();

	float fps_;
	float currentGameTime;
	float lastDeltaTime;

	/************************************************************************/
	/*                               BOX2D                                  */
	/************************************************************************/
	b2World* b2dWorld;
	b2Vec2 gravity;


	/************************************************************************/
	/*                               SOUND                                  */
	/************************************************************************/
	gef::AudioManager* audioManager;
	SoundMap sounds;
	int masterVolume = 30.f;

	PlayerController* soundController;

	/************************************************************************/
	/*                               RANDOM                                 */
	/************************************************************************/
	std::default_random_engine rd;
	std::uniform_real_distribution<float> dist;

	/************************************************************************/
	/*                             PARTICLES                                */
	/************************************************************************/
	ParticleManager* particleManager;

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
	gef::AudioManager* GetAudioManager() { return audioManager; }
	const gef::Vector2 GetLastTouchPosition();

	/* Game State */
	void SetGameState(GameState::Type newState);
	GameState::Type GetGameState() const { return gameState; }

	/* Player money */
	const uint32_t GetPlayerMoney() { return playerMoney; }
	void IncreasePlayerMoney(uint32_t increaseAmount) { playerMoney += increaseAmount; }
	void ApplyCostToPlayerMoney(uint32_t cost) { playerMoney -= cost; }

	/* Camera Shake */

	/// <summary>
	/// Applies camera shake
	/// @NOTE: This overwrites the previous camera shake
	/// </summary>
	void ApplyCameraShake(FCameraShake newShake);

	/* Damage */
	void ApplyRadialDamage(float damageAmount, gef::Vector4 origin, float innerRadius, float outerRadius);

	/* Sound */
	void PlaySample(std::string sampleName, float pitch = 1, bool bIsLooping = false);

	/* Pointer functions */
	void AddPointerLocationOffset(gef::Vector2 offset) { pointerLocation += offset; }

	void SetPointerLocation(gef::Vector2 newLocation) { pointerLocation = newLocation; }

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
	SpriteActorType* SpawnSpriteActor(gef::Sprite* sprite = nullptr, gef::Vector4 position = gef::Vector4::kZero, float rotation = 0.f, WorldObject* owner = nullptr)
	{
		SpriteActorType* spriteActor = new SpriteActorType();
		spriteActor->SetSprite(sprite);
		spriteActor->SetTranslation(position);
		spriteActor->SetRotation(rotation);
		spriteActor->Init(owner);
		spriteActors.push_back(spriteActor);

		return spriteActor;
	}
};

#endif // _SCENE_APP_H

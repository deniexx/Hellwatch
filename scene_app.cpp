#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include "graphics/sprite.h"
#include "assets/png_loader.h"
#include <graphics/image_data.h>
#include <audio/audio_manager.h>

#include "UserInterface/MainMenu.h"
#include "UserInterface/ShopMenu.h"
#include "UserInterface/PauseMenu.h"
#include "UserInterface/GameEndMenu.h"
#include "GameFramework/PlayerController.h"
#include "Actors/MeshActors/PlayerCharacter.h"
#include "Actors/SpriteActor.h"
#include "Actors/MeshActors/EnemyDummy.h"
#include "Actors/MeshActors/Enemy.h"
#include "Actors/MeshActors/RangedEnemy.h"
#include "Actors/MeshActor.h"
#include "GameFramework/WaveManager.h"
#include <thread>

SceneApp::SceneApp(gef::Platform& platform):
	Application(platform)
	,sprite_renderer_(NULL)
	,renderer_3d_(NULL)
	,primitive_builder_(NULL)
	,font_(NULL)
	,scene_assets_(NULL)
	,mainMenu(NULL)
	,shopMenu(NULL)
	,pauseMenu(NULL)
	,gameEndMenu(NULL)
	,audioManager(NULL)
	,currentGameTime(0.f)
{
}

#pragma region Init and CleanUp
void SceneApp::Init()
{
	SetGameState(GameState::Loading);

	if (!instance)
		instance = this;
	else
		delete this;

	BuildToLoadData();

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	gravity = b2Vec2(0.0f, -0.f);
	b2dWorld = new b2World(gravity);
	audioManager = gef::AudioManager::Create();
	// @TODO: Find samples and music

	InitFont();
	SetupLights();

	loadingSprite = new gef::Sprite();

	gef::PNGLoader png_loader;
	gef::ImageData imageData;
	png_loader.Load("Assets/Loading.png", platform_, imageData);
	if (imageData.image() != nullptr)
	{
		gef::Texture* texture = gef::Texture::Create(platform_, imageData);
		loadingSprite->set_position(platform_.width() / 2, platform_.height() / 2, 0.f);
		loadingSprite->set_width(1920);
		loadingSprite->set_height(1080);
		loadingSprite->set_texture(texture);
	}

	loadFuture = std::async(std::launch::async, [this] { LoadAssets(); return GameState::Type::Loading; });
}

void SceneApp::InitMainMenu()
{
	if (mainMenu == NULL)
	{
		mainMenu = new MainMenu();
		mainMenu->Init();
	}

	if (bGameLoopInitted)
		bCommingFromMainMenu = true;
}

void SceneApp::InitShop()
{
	if (shopMenu == NULL)
	{
		shopMenu = new ShopMenu();
		shopMenu->Init();
	}
}

void SceneApp::InitGameLoop()
{
	if (bGameLoopInitted && !bCommingFromMainMenu) 
	{
		return;
	}
	else if (bGameLoopInitted && bCommingFromMainMenu)
	{
		playerCharacter = NULL;
		delete waveManager;
		delete primitive_builder_;

		for (int i = 0; i < meshActors.size(); ++i)
		{
			meshActors[i]->MarkForDelete();
		}

		for (int i = 0; i < spriteActors.size(); ++i)
		{
			spriteActors[i]->MarkForDelete();
		}

		playerMoney = 0;
		delete shopMenu;
		shopMenu = NULL;
		bCommingFromMainMenu = false;
	}

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	playerCharacter = SpawnMeshActor<PlayerCharacter>();

	waveManager = new WaveManager();
	waveManager->Init();

	b2BodyDef newBodyDef;
	newBodyDef.type = b2_staticBody;
	newBodyDef.position.Set(0.f, -2.f);

	b2PolygonShape newDynamicBox;
	newDynamicBox.SetAsBox(10.f, 1.f);

	b2FixtureDef newFixtureDef;
	newFixtureDef.shape = &newDynamicBox;
	newFixtureDef.density = 1.f;
	newFixtureDef.friction = 0.3f;

	// Build environment
	gef::Material mat;
	MeshActor* actor = SpawnMeshActor(primitive_builder_->CreateBoxMesh(gef::Vector4(50.f, 0.5f, 50.f)), gef::Vector4(0.f, -2.f, 0.f));
	mat.set_colour(0xFFFFFFFF);
	mat.set_texture(RequestTextureByName("Environment"));
	actor->SetMaterial(mat);

	std::vector<b2Vec2> blockPositions = { b2Vec2(12.f, 12.f)
									, b2Vec2(-12.f, -12.f)
									, b2Vec2(-12.f, 12.f)
									, b2Vec2(12.f, -12.f)
									, b2Vec2(20.f, 0.f)
									, b2Vec2(-20.f, 0.f)
									, b2Vec2(0.f, 20.f)
									, b2Vec2(0.f, -20.f)
									, b2Vec2(21.f, 21.f)
									, b2Vec2(21.f, -21.f)
									, b2Vec2(-21.f, -21.f)
									, b2Vec2(-21.f, 21.f) };

	b2BodyDef envBodyDef;
	b2FixtureDef envFixture;

	for (int i = 0; i < blockPositions.size(); ++i)
	{
		float x = blockPositions[i].x;
		float y = blockPositions[i].y;
		MeshActor* environment = SpawnMeshActor(RequestMeshByName("EnvironmentBlock"), gef::Vector4(x, 0.f, y));
		environment->SetMaterial(mat);

		envBodyDef.type = b2_staticBody;
		envBodyDef.position.Set(x, y);

		b2CircleShape shape;
		shape.m_radius = 1.f;

		envFixture.shape = &shape;
		envFixture.density = 1.f;

		environment->ID = ENVIRONMENT_ID;
		environment->SetCollisionBody(CreateCollisionBody(envBodyDef, envFixture, environment));
	}

	 // Left and Right walls
	gef::Material blockMat;
	blockMat.set_colour(0xFF000000);

	MeshActor* blockingWall = SpawnMeshActor(RequestMeshByName("BlockingWall"), gef::Vector4(30.f, 0.f, 0.f));
	envBodyDef.position.Set(30.f, 0.f);
	b2PolygonShape envShape;
	envShape.SetAsBox(1.f, 60.f);
	envFixture.shape = &envShape;
	blockingWall->SetCollisionBody(CreateCollisionBody(envBodyDef, envFixture, blockingWall));
	blockingWall->SetMaterial(blockMat);
	blockingWall->ID = ENVIRONMENT_ID;

	blockingWall = SpawnMeshActor(RequestMeshByName("BlockingWall"), gef::Vector4(-30.f, 0.f, 0.f));
	envBodyDef.position.Set(-30.f, 0.f);
	blockingWall->SetCollisionBody(CreateCollisionBody(envBodyDef, envFixture, blockingWall));
	blockingWall->SetMaterial(blockMat);
	blockingWall->ID = ENVIRONMENT_ID;

	// Up and down
	blockingWall = SpawnMeshActor(RequestMeshByName("BlockingWallY"), gef::Vector4(0.f, 0.f, 30.f));
	envBodyDef.position.Set(0.f, 30.f);
	envShape.SetAsBox(60.f, 1.f);
	envFixture.shape = &envShape;
	blockingWall->SetCollisionBody(CreateCollisionBody(envBodyDef, envFixture, blockingWall));
	blockingWall->SetMaterial(blockMat);
	blockingWall->ID = ENVIRONMENT_ID;

	blockingWall = SpawnMeshActor(RequestMeshByName("BlockingWallY"), gef::Vector4(0.f, 0.f, -30.f));
	envBodyDef.position.Set(0.f, -30.f);
	blockingWall->SetCollisionBody(CreateCollisionBody(envBodyDef, envFixture, blockingWall));
	blockingWall->SetMaterial(blockMat);
	blockingWall->ID = ENVIRONMENT_ID;

	bGameLoopInitted = true;
}

void SceneApp::InitPauseMenu()
{
	if (pauseMenu == NULL)
	{
		pauseMenu = new PauseMenu();
		pauseMenu->Init();
	}
}

void SceneApp::InitGameEndMenu()
{
	if (gameEndMenu == NULL)
	{
		gameEndMenu = new GameEndMenu();
		gameEndMenu->Init();
	}
}

void SceneApp::CleanUp()
{
	// clean up scene assets
	delete scene_assets_;
	scene_assets_ = NULL;

	CleanUpFont();

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete b2dWorld;
	b2dWorld = NULL;

	delete mainMenu;
	mainMenu = NULL;
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");

}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}
#pragma endregion

#pragma region GameLoop
bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	currentGameTime += frame_time;
	lastDeltaTime = frame_time;

	switch (gameState)
	{
	case GameState::Loading:
		UpdateLoading(frame_time);
		break;
	case GameState::MainMenu:
		UpdateMainMenu(frame_time);
		break;
	case GameState::GameLoop:
		UpdateGameLoop(frame_time);
		break;
	case GameState::PauseMenu:
		UpdatePauseMenu(frame_time);
		break;
	case GameState::Shop:
		UpdateShop();
		break;
	case GameState::GameEnd:
		UpdateGameEnd();
		break;
	}

	return true;
}

void SceneApp::UpdateLoading(float frame_time)
{
	if (loadFuture._Is_ready())
	{
		SetGameState(GameState::MainMenu);
		delete loadingSprite;
	}
}

void SceneApp::UpdateMainMenu(float frame_time)
{
	mainMenu->Update();
}

void SceneApp::UpdateGameLoop(float frame_time)
{
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	b2dWorld->Step(frame_time, velocityIterations, positionIterations);
	waveManager->Update();
	HandleCollision();

	for (int i = 0; i < meshActors.size(); ++i)
	{
		if (meshActors[i] && meshActors[i]->ShouldUpdate())
			meshActors[i]->Update(frame_time);
	}

	for (int i = 0; i < spriteActors.size(); ++i)
	{
		if (spriteActors[i] && spriteActors[i]->ShouldUpdate())
			spriteActors[i]->Update(frame_time);
	}

	UpdateShop();
	CheckMarkedForDeletion();
}

void SceneApp::UpdateShop()
{
	if (shopMenu)
		shopMenu->Update();
}

void SceneApp::UpdateGameEnd()
{
	if (gameEndMenu)
		gameEndMenu->Update();
}

void SceneApp::UpdatePauseMenu(float frame_time)
{
	if (pauseMenu)
		pauseMenu->Update();
}

void SceneApp::Render()
{
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.001f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	switch (gameState)
	{
	case GameState::Loading:
		RenderLoading();
		break;
	case GameState::MainMenu:
		RenderMainMenu();
		break;
	case GameState::GameLoop:
		RenderGameLoop();
		break;
	case GameState::PauseMenu:
		RenderPauseMenu();
		break;
	case GameState::Shop:
		RenderShop();
		break;
	case GameState::GameEnd:
		RenderGameEnd();
		break;
	}
}

void SceneApp::RenderLoading()
{
	cameraLookAt = gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*loadingSprite);
	sprite_renderer_->End();
}

void SceneApp::RenderShop()
{
	if (playerCharacter != NULL)
	{
		cameraEye = playerCharacter->GetTranslation();
		cameraEye.set_y(20.f);
		cameraEye.set_z(cameraEye.z() - 10);
		cameraLookAt = playerCharacter->GetTranslation();
		cameraLookAt.set_y(-1.f);
	}

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	for (auto actor : meshActors)
		actor->Render();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);

	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	shopMenu->DrawMenuHUD(font_, sprite_renderer_);
	sprite_renderer_->End();
}

void SceneApp::RenderGameEnd()
{
	if (playerCharacter != NULL)
	{
		cameraEye = playerCharacter->GetTranslation();
		cameraEye.set_y(20.f);
		cameraEye.set_z(cameraEye.z() - 10);
		cameraLookAt = playerCharacter->GetTranslation();
		cameraLookAt.set_y(-1.f);
	}

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	for (auto actor : meshActors)
		actor->Render();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);

	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	gameEndMenu->DrawMenuHUD(font_, sprite_renderer_);
	sprite_renderer_->End();
}

void SceneApp::RenderMainMenu()
{
	cameraLookAt = gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*mainMenu->menuSprite);
	sprite_renderer_->End();

	if (font_)
	{
		mainMenu->DrawMenuHUD(font_, sprite_renderer_);
	}
}

void SceneApp::RenderGameLoop()
{
	if (playerCharacter != NULL)
	{
		cameraEye = playerCharacter->GetTranslation();
		cameraEye.set_y(20.f);
		cameraEye.set_z(cameraEye.z() - 10);
		cameraLookAt = playerCharacter->GetTranslation();
		cameraLookAt.set_y(-1.f);
	}

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	for (auto actor : meshActors)
		actor->Render();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);

	playerCharacter->DrawPlayerHUD();
	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	sprite_renderer_->End();
}

void SceneApp::RenderPauseMenu()
{
	if (playerCharacter != NULL)
	{
		cameraEye = playerCharacter->GetTranslation();
		cameraEye.set_y(20.f);
		cameraEye.set_z(cameraEye.z() - 10);
		cameraLookAt = playerCharacter->GetTranslation();
		cameraLookAt.set_y(-1.f);
	}

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	for (auto actor : meshActors)
		actor->Render();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);

	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	pauseMenu->DrawMenuHUD(font_, sprite_renderer_);
	sprite_renderer_->End();
}

void SceneApp::HandleCollision()
{
	b2Contact* contact = b2dWorld->GetContactList();
	int numContacts = b2dWorld->GetContactCount();

	for (int i = 0; i < numContacts; ++i)
	{
		if (contact->IsTouching())
		{
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			WorldObject* wo = (WorldObject*)(bodyA->GetUserData().pointer);
			if (wo != nullptr)
				wo->OnCollision(bodyB);

			wo = (WorldObject*)(bodyB->GetUserData().pointer);
			if (wo != nullptr)
				wo->OnCollision(bodyA);
		}

		contact->GetNext();
	}
}

void SceneApp::CheckMarkedForDeletion()
{
	std::vector<MeshActor*> cleanMeshActors;
	std::vector<SpriteActor*> cleanSpriteActors;

	std::vector<WorldObject*> toBeDeleted;

	for (auto actor : meshActors)
	{
		if (actor->GetIsMarkedForDelete())
			toBeDeleted.push_back(actor);
		else
			cleanMeshActors.push_back(actor);
	}

	for (auto actor : spriteActors)
	{
		if (actor->GetIsMarkedForDelete())
			toBeDeleted.push_back(actor);
		else
			cleanSpriteActors.push_back(actor);
	}


	meshActors = cleanMeshActors;
	spriteActors = cleanSpriteActors;

	for (int i = 0; i < toBeDeleted.size(); ++i)
		delete toBeDeleted[i];
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		gef::Vector4 mousePos = ProjectScreenToWorldSpace(GetLastTouchPosition());
		gef::Vector4 characterPos = playerCharacter ? playerCharacter->GetTranslation() : gef::Vector4::kZero;
		font_->RenderText(sprite_renderer_, gef::Vector4(1800.0f, 1040.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, gef::Vector4(1740.0f, 1000.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Mouse: %.1f, %.1f", mousePos.x(), mousePos.z());
		font_->RenderText(sprite_renderer_, gef::Vector4(1700.0f, 960.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Character: %.1f, %.1f", characterPos.x(), characterPos.z());
	}
}
#pragma endregion

#pragma region SceneAssets
gef::Scene* SceneApp::LoadSceneAssets(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();

	if (scene->ReadSceneFromFile(platform, filename))
	{
		// if scene file loads successful
		// create material and mesh resources from the scene data
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
	{
		delete scene;
		scene = NULL;
	}

	return scene;
}

gef::Mesh* SceneApp::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

void SceneApp::LoadAssets()
{
	OBJMeshLoader meshLoader;
	for (const auto& mesh : meshesToLoad)
		meshLoader.Load(mesh.c_str(), platform_, meshes);

	gef::PNGLoader png_loader;
	gef::ImageData imageData;
	for (const auto& text : texturesToLoad)
	{
		png_loader.Load(text.second.c_str(), platform_, imageData);
		if (imageData.image() != nullptr)
		{
			gef::Texture* texture = gef::Texture::Create(platform_, imageData);
			textures[text.first] = texture;
		}
	}
}

void SceneApp::BuildToLoadData()
{
	meshesToLoad.push_back("Assets/MainCharacter.obj");
	meshesToLoad.push_back("Assets/IceBolt.obj");
	meshesToLoad.push_back("Assets/EnvironmentBlock.obj");
	meshesToLoad.push_back("Assets/BlockingWalls.obj");
	meshesToLoad.push_back("Assets/MeleeEnemy.obj");
	meshesToLoad.push_back("Assets/RangedEnemy.obj");
	meshesToLoad.push_back("Assets/Meteor.obj");

	texturesToLoad["Ganfaul"] = "Assets/Ganfaul_diffuse.png";
	texturesToLoad["Environment"] = "Assets/Environment_diffuse.png";
	texturesToLoad["IceBolt"] = "Assets/IceBolt_diffuse.png";
	texturesToLoad["BlockingWall"] = "Assets/BlockingWall_diffuse.png";
	texturesToLoad["MeleeEnemy"] = "Assets/MeleeEnemy_diffuse.png";
	texturesToLoad["RangedEnemy"] = "Assets/RangedEnemy_diffuse.png";
	texturesToLoad["Meteor"] = "Assets/Meteor_diffuse.png";
}

gef::Mesh* SceneApp::RequestMeshByName(std::string meshName)
{
	auto it = meshes.find(meshName);
	if (it != meshes.end())
	{
		return meshes[meshName];
	}

	return nullptr;
}

gef::Texture* SceneApp::RequestTextureByName(std::string textureName)
{
	auto it = textures.find(textureName);
	if (it != textures.end())
	{
		return textures[textureName];
	}

	return nullptr;
}
#pragma endregion

#pragma region Others
b2Body* SceneApp::CreateCollisionBody(b2BodyDef bodyDef, b2FixtureDef fixtureDef, WorldObject* owningObject)
{
	b2Body* body = b2dWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	b2BodyUserData& userData = body->GetUserData();
	userData.pointer = (uintptr_t)owningObject;

	return body;
}

const gef::Vector2 SceneApp::GetLastTouchPosition()
{
	return SceneApp::instance->GetPlayerCharacter()->GetController()->GetMousePosition();
}

void SceneApp::SetGameState(GameState::Type newState)
{
	switch (newState)
	{
	case GameState::MainMenu:
	{
		InitMainMenu();
	}
	break;
	case GameState::GameLoop:
	{
		if (gameState == GameState::Shop)
		{
			waveManager->StartWave();
		}
		InitGameLoop();
	}
	break;
	case GameState::PauseMenu:
	{
		InitPauseMenu();
	}
	break;
	case GameState::Shop:
	{
		InitShop();
	}
	break;
	case GameState::GameEnd:
	{
		InitGameEndMenu();
	}
	break;
	}

	gameState = newState;
}

void SceneApp::ApplyRadialDamage(float damageAmount, gef::Vector4 origin, float innerRadius, float outerRadius)
{
	std::vector<Enemy*> enemies = waveManager->GetAllSpawnedEnemies();

	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i] && !enemies[i]->GetIsMarkedForDelete())
		{
			float distance = (enemies[i]->GetTranslation() - origin).Length();
			if (distance < innerRadius)
			{
				enemies[i]->TakeDamage(damageAmount);
			}
			else if (distance < outerRadius)
			{
				enemies[i]->TakeDamage(gef::Lerp(damageAmount, 1, distance / outerRadius));
			}
		}
	}
}

int SceneApp::FindSampleFromName(std::string sampleName)
{
	auto it = sounds.find(sampleName);
	if (it != sounds.end())
	{
		return sounds[sampleName];
	}

	return -1;
}

void SceneApp::PlaySample(std::string sampleName, bool bIsLooping)
{
	int sampleIndex = FindSampleFromName(sampleName);

	if (sampleIndex >= 0)
	{
		audioManager->PlaySample(sampleIndex, bIsLooping);
	}
}
#pragma endregion
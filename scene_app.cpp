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

#include "GameFramework/PlayerController.h"
#include "Actors/MeshActors/PlayerCharacter.h"
#include "Actors/SpriteActor.h"
#include "Actors/MeshActors/EnemyDummy.h"
#include "Actors/MeshActors/Enemy.h"
#include "Actors/MeshActors/RangedEnemy.h"
#include "Actors/MeshActor.h"
#include <thread>

SceneApp::SceneApp(gef::Platform& platform):
	Application(platform)
	,sprite_renderer_(NULL)
	,renderer_3d_(NULL)
	,primitive_builder_(NULL)
	,font_(NULL)
	,scene_assets_(NULL)
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
	MenuButton startButton;
	startButton.buttonText = "Start";
	startButton.color = 0xFF0000FF;
	startButton.justification = gef::TJ_LEFT;
	startButton.scale = 3.f;
	startButton.position = gef::Vector4(160.f, 540.f, 0.f);
	startButton.callbackFunction = bindFunc(OnStartButtonClicked);
	mainMenuButtons.push_back(startButton);

	MenuButton exitButton;
	exitButton.buttonText = "Exit";
	exitButton.color = 0xFF0000FF;
	exitButton.justification = gef::TJ_LEFT;
	exitButton.scale = 3.f;
	exitButton.position = gef::Vector4(160.f, 640.f, 0.f);
	exitButton.callbackFunction = bindFunc(OnExitButtonClicked);
	mainMenuButtons.push_back(exitButton);

	mainMenuSprite = new gef::Sprite();
	gef::PNGLoader png_loader;
	gef::ImageData imageData;
	png_loader.Load("Assets/MainMenu.png", platform_, imageData);
	if (imageData.image() != nullptr)
	{
		gef::Texture* texture = gef::Texture::Create(platform_, imageData);
		mainMenuSprite->set_position(platform_.width() / 2, platform_.height() / 2, 0.f);
		mainMenuSprite->set_width(1920);
		mainMenuSprite->set_height(1080);
		mainMenuSprite->set_texture(texture);
	}

	menuController = new PlayerController(platform_);
	FKeyBindKeyboard keyboardKeybind;

	// Move down on menu keybinds
	keyboardKeybind.inputAction = HellwatchInputAction::Released;
	keyboardKeybind.keyCode = gef::Keyboard::KC_S;
	keyboardKeybind.functionBind = bindFunc(OnDownButtonPressed);
	menuController->BindKeyboardEvent(keyboardKeybind);

	keyboardKeybind.keyCode = gef::Keyboard::KC_DOWN;
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Move up on menu keybinds
	keyboardKeybind.keyCode = gef::Keyboard::KC_UP;
	keyboardKeybind.functionBind = bindFunc(OnUpButtonPressed);
	menuController->BindKeyboardEvent(keyboardKeybind);

	keyboardKeybind.keyCode = gef::Keyboard::KC_W;
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Submit keybind
	keyboardKeybind.keyCode = gef::Keyboard::KC_RETURN;
	keyboardKeybind.functionBind = bindFunc(PressMenuButton);
	menuController->BindKeyboardEvent(keyboardKeybind);

	// Mouse click to submit keybind
	FKeyBindMouse mouseKeybind;
	mouseKeybind.clickAction = gef::TT_NEW;
	mouseKeybind.functionBind = bindFunc_ONEParam(OnMouseButtonPressed, gef::Vector2, mousePos);
	menuController->BindMouseEvent(mouseKeybind);
}

void SceneApp::InitGameLoop()
{
	if (bGameLoopInitted) return;

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	playerCharacter = SpawnMeshActor<PlayerCharacter>();

	gef::Mesh* mesh = primitive_builder_->CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
	enemyDummy = SpawnMeshActor<EnemyDummy>(mesh, gef::Vector4(2.0f, 0.f, 2.0f));
	testEnemy = SpawnMeshActor<Enemy>(mesh, gef::Vector4(4.0f, 0.f, 4.0f));
	testRanged = SpawnMeshActor<RangedEnemy>(mesh, gef::Vector4(10.0f, 0.f, 10.0f));

	gef::Material mat;
	mat.set_colour(0xFF0000FF);
	testEnemy->SetMaterial(mat);
	testRanged->SetMaterial(mat);

	b2BodyDef newBodyDef;
	newBodyDef.type = b2_staticBody;
	newBodyDef.position.Set(0.f, -2.f);

	b2PolygonShape newDynamicBox;
	newDynamicBox.SetAsBox(10.f, 1.f);

	b2FixtureDef newFixtureDef;
	newFixtureDef.shape = &newDynamicBox;
	newFixtureDef.density = 1.f;
	newFixtureDef.friction = 0.3f;

	MeshActor* actor = SpawnMeshActor(primitive_builder_->CreateBoxMesh(gef::Vector4(50.f, 0.5f, 50.f)), gef::Vector4(0.f, -2.f, 0.f));
	gef::Material enemyMat;
	mat.set_colour(0xFF00FFFF);
	actor->SetMaterial(mat);

	bGameLoopInitted = true;
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
	if (menuController)
	{
		menuController->Update();
		CheckForHighlight();
	}
}

void SceneApp::UpdateGameLoop(float frame_time)
{
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	b2dWorld->Step(frame_time, velocityIterations, positionIterations);
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

	CheckMarkedForDeletion();
}

void SceneApp::UpdatePauseMenu(float frame_time)
{

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

void SceneApp::RenderMainMenu()
{
	cameraLookAt = gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cameraEye, cameraLookAt, cameraUp);
	renderer_3d_->set_view_matrix(view_matrix);

	renderer_3d_->Begin();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*mainMenuSprite);
	sprite_renderer_->End();

	if (font_)
	{
		gef::Vector2 mousePos = menuController->GetMousePosition();
		font_->RenderText(sprite_renderer_, gef::Vector4(1700.0f, 1000.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Mouse: %.1f, %.1f", mousePos.x, mousePos.y);

		for (int i = 0; i < mainMenuButtons.size(); ++i)
		{
			float btnScale = i == currentButtonFocused ? mainMenuButtons[i].scale * 1.2f : mainMenuButtons[i].scale;
			font_->RenderText(sprite_renderer_, mainMenuButtons[i].position, btnScale, mainMenuButtons[i].color, mainMenuButtons[i].justification, mainMenuButtons[i].buttonText.c_str());
		}
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

	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	sprite_renderer_->End();
}

void SceneApp::RenderPauseMenu()
{

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
		font_->RenderText(sprite_renderer_, gef::Vector4(1700.0f, 920.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Enemy Health: %.1f", enemyDummy->GetHealth());
	}
}
#pragma endregion

#pragma region MainMenuButtons
void SceneApp::OnDownButtonPressed()
{
	currentButtonFocused = (currentButtonFocused + 1) % mainMenuButtons.size();
}

void SceneApp::OnUpButtonPressed()
{
	currentButtonFocused = (currentButtonFocused - 1) % mainMenuButtons.size();
}

void SceneApp::OnMouseButtonPressed(gef::Vector2 mousePos)
{
	if (mousePos.x > 140 && mousePos.x < 340 && mousePos.y > 540 && mousePos.y < 630)
	{
		OnStartButtonClicked();
	}
	else if (mousePos.x > 140 && mousePos.x < 340 && mousePos.y > 640 && mousePos.y < 730)
	{
		OnExitButtonClicked();
	}
}

void SceneApp::OnControllerDownButton(gef::Vector2 dir)
{
	currentButtonFocused = (currentButtonFocused + 1) % mainMenuButtons.size();
}


void SceneApp::OnControllerUpButton(gef::Vector2 dir)
{
	currentButtonFocused = (currentButtonFocused - 1) % mainMenuButtons.size();
}

void SceneApp::CheckForHighlight()
{
	gef::Vector2 mousePos = menuController->GetMousePosition();
	
	if (mousePos.x > 140 && mousePos.x < 340 && mousePos.y > 540 && mousePos.y < 630)
	{
		currentButtonFocused = 0;
	}
	else if (mousePos.x > 140 && mousePos.x < 340 && mousePos.y > 640 && mousePos.y < 730)
	{
		currentButtonFocused = 1;
	}
}

void SceneApp::OnStartButtonClicked()
{
	SetGameState(GameState::GameLoop);
}

void SceneApp::OnExitButtonClicked()
{
	exit(0);
}

void SceneApp::PressMenuButton()
{
	mainMenuButtons[currentButtonFocused].callbackFunction();
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
#pragma endregion

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
	texturesToLoad["Ganfaul"] = "Assets/Ganfaul_diffuse.png";
}

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

void SceneApp::SetGameState(GameState::Type newState)
{
	switch (newState)
	{
	case GameState::Loading:
	break;
	case GameState::MainMenu:
	{
		InitMainMenu();
	}
	break;
	case GameState::GameLoop:
	{
		InitGameLoop();
	}
	break;
	case GameState::PauseMenu:
	{

	}
	break;
	}

	gameState = newState;
}
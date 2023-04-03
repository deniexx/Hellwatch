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
#include "obj_mesh_loader.h"

#include "Actors/MeshActors/PlayerCharacter.h"
#include <Actors/SpriteActor.h>
#include "Actors/MeshActors/EnemyDummy.h"
#include <Actors/MeshActor.h>

SceneApp::SceneApp(gef::Platform& platform):
	Application(platform)
	,sprite_renderer_(NULL)
	,renderer_3d_(NULL)
	,primitive_builder_(NULL)
	,font_(NULL)
	,scene_assets_(NULL)
{
}

void SceneApp::Init()
{
	if (!instance)
		instance = this;
	else
		delete this;

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	gravity = b2Vec2(0.0f, -0.f);
	b2dWorld = new b2World(gravity);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	playerCharacter = SpawnMeshActor<PlayerCharacter>();

	gef::Mesh* mesh = primitive_builder_->CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
	enemyDummy = SpawnMeshActor<EnemyDummy>(mesh, gef::Vector4(2.0f, 0.f, 2.0f));
	testEnemy = SpawnMeshActor<Enemy>(mesh, gef::Vector4(2.0f, 0.f, 2.0f));


	b2BodyDef newBodyDef;
	newBodyDef.type = b2_staticBody;
	newBodyDef.position.Set(0.f, -2.f);

	b2PolygonShape newDynamicBox;
	newDynamicBox.SetAsBox(10.f, 1.f);

	b2FixtureDef newFixtureDef;
	newFixtureDef.shape = &newDynamicBox;
	newFixtureDef.density = 1.f;
	newFixtureDef.friction = 0.3f;

	gef::Material mat1;
	mat1.set_colour(0xFF0000FF);
	playerCharacter->SetMaterial(mat1);

	MeshActor* actor = SpawnMeshActor(primitive_builder_->CreateBoxMesh(gef::Vector4(50.f, 0.5f, 50.f)), gef::Vector4(0.f, -2.f, 0.f));
	gef::Material mat;
	mat.set_colour(0xFF00FFFF);
	actor->SetMaterial(mat);

	InitFont();
	SetupLights();
	LoadAssets();
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

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	b2dWorld->Step(frame_time, velocityIterations, positionIterations);
	HandleCollision();

	for (int i = 0; i < meshActors.size(); ++i)
	{
		if (meshActors[i])
			meshActors[i]->Update(frame_time);
	}
		
	for (int i = 0; i < spriteActors.size(); ++i)
	{
		if (spriteActors[i])
			spriteActors[i]->Update(frame_time);
	}

	CheckMarkedForDeletion();

	return true;
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

void SceneApp::Render()
{
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.001f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	if (playerCharacter)
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


	// draw 3d geometry
	renderer_3d_->Begin();

	for (auto actor : meshActors)
		actor->Render();

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	
	DrawHUD();
	for (auto actor : spriteActors)
		actor->Render();

	sprite_renderer_->End();
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

void SceneApp::LoadAssets()
{
	//gef::Scene* scene = LoadSceneAssets(platform_, "Assets/MainCharacter.scn");

	//playerCharacter->SetMesh(GetMeshFromSceneAssets(scene));

	OBJMeshLoader meshLoader;

	MeshMap map;

	if (meshLoader.Load("Assets/MainCharacter.obj", platform_, map))
	{
		playerCharacter->SetMesh(map["Ganfaul"]);
	}
	else
	{
		std::string result = meshLoader.GetLastError();
		gef::DebugOut(result.c_str());

		gef::Mesh* mesh = primitive_builder_->CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
		playerCharacter->SetMesh(mesh);
	}

	gef::PNGLoader png_loader;
	gef::ImageData imageData;

	png_loader.Load("Assets/Ganfaul_diffuse.png", platform_, imageData);
	gef::Material mat;
	if (imageData.image() != nullptr)
	{
		gef::Texture* texture = gef::Texture::Create(platform_, imageData);
		mat.set_texture(texture);
	}

	playerCharacter->SetMaterial(mat);
	playerCharacter->SetScale(gef::Vector4(1.f, 1.f, 1.f));
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
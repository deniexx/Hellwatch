#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include "graphics/sprite.h"

#include "Actors/MeshActors/PlayerCharacter.h"
#include <Actors/SpriteActor.h>
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

	std::vector<MeshActor*> cleanMeshActors;
	std::vector<SpriteActor*> cleanSpriteActors;

	for (auto actor : meshActors)
	{
		if (actor)
		{
			actor->Update(frame_time);
			cleanMeshActors.push_back(actor);
		}
	}
		
	for (auto actor : spriteActors)
	{
		if (actor)
		{
			actor->Update(frame_time);
			cleanSpriteActors.push_back(actor);
		}
	}

	meshActors = cleanMeshActors;
	spriteActors = cleanSpriteActors;

	return true;
}

void SceneApp::Render()
{
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.001f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye =  gef::Vector4(0.0f, 20.0f, 0.0f);
	gef::Vector4 camera_lookat = gef::Vector4(0.0f, -1.0f, 0.001f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);

	if (playerCharacter)
	{
		camera_eye = playerCharacter->GetTranslation();
		camera_eye.set_y(20.f);
		camera_eye.set_z(camera_eye.z() - 10);
		camera_lookat = playerCharacter->GetTranslation();
		camera_lookat.set_y(-1.f);
	}
	
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
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
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(1800.0f, 1040.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
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

			if (WorldObject* wo = (WorldObject*)bodyA->GetUserData().pointer)
				wo->OnCollision(bodyB);
			
			if (WorldObject* wo = (WorldObject*)bodyB->GetUserData().pointer)
				wo->OnCollision(bodyA);
		}
	}
}

void SceneApp::LoadAssets()
{
	gef::Scene* scene = LoadSceneAssets(platform_, "Assets/MainCharacter.scn");

	playerCharacter->SetMesh(GetMeshFromSceneAssets(scene));
	playerCharacter->SetMaterial(*scene->materials.front());
	playerCharacter->SetScale(gef::Vector4(0.01f, 0.01f, 0.01f));
}

b2Body* SceneApp::CreateCollisionBody(b2BodyDef bodyDef, b2FixtureDef fixtureDef, WorldObject* owningObject)
{
	b2Body* body = b2dWorld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	b2BodyUserData& userData = body->GetUserData();
	userData.pointer = (uintptr_t)owningObject;

	return body;
}
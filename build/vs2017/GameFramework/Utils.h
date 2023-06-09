#pragma once

#include "maths/Vector2.h"
#include "maths/vector4.h"
#include "maths/matrix44.h"
#include <graphics/renderer_3d.h>
#include <scene_app.h>
#include "maths/math_utils.h"

#define NO_ID 0
#define PLAYER_ID 1
#define ENEMY_ID 2
#define ENVIRONMENT_ID 3

#define PI 3.14159265358979323846

#define GENERATED_BODY(ParentClass, CurrentClass) typedef ::ParentClass Super; \
									typedef CurrentClass ThisClass;


#define bindFunc(funcName) [this]() { funcName(); }
#define bindFunc_ONEParam(funcName, paramType, paramName) [this](paramType paramName) { funcName(paramName); }
#define bindFunc_TWOParams(funcName, paramType1, paramName1, paramType2, paramName2) [this](paramType1 paramName1, paramType2 paramName2) { funcName(paramName1, paramName2); }
#define bindFunc_THREEParam(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3) { funcName(paramName1, paramName2, paramName3); }
#define bindFunc_FOURParams(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3, paramType4, paramName4) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3, paramType4 paramName4) { funcName(paramName1, paramName2, paramName3, paramName4); }
#define bindFunc_FIVEParams(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3, paramType4, paramName4, paramType5, paramName5) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3, paramType4 paramName4, paramName5 paramName5) { funcName(paramName1, paramName2, paramName3, paramName4, paramName5); }

/// <summary>
/// Clamps a value between 2 given values
/// </summary>
template<typename T>
static T Clamp(T expression, T minVal, T maxVal)
{
	if (expression < minVal)
		expression = minVal;
	
	return expression > maxVal ? maxVal : expression;
}

/// <summary>
/// Projects a Vector2 into world space
/// </summary>
static gef::Vector4 ProjectScreenToWorldSpace(gef::Vector2 v2)
{
	gef::Renderer3D* renderer = SceneApp::instance->GetRenderer3D();
	float vpWidth = SceneApp::instance->GetViewportWidth();
	float vpHeight = SceneApp::instance->GetViewportHeight();

	gef::Matrix44 matProjection = renderer->projection_matrix() * renderer->view_matrix();
	matProjection.Inverse(matProjection);

	gef::Vector4 in;
	float winZ = 1.f;

	in.set_x((2.f * (float)(v2.x) / (vpWidth)) - 1);
	in.set_y(-1.f);
	in.set_z(1.f - (2.f * (float)(v2.y) / (vpHeight)));
	in.set_w(1.f);

	gef::Matrix44 calc;
	calc.SetIdentity();

	calc.SetTranslation(in);

	gef::Matrix44 actualPos = calc * matProjection;

	gef::Vector4 pos = actualPos.GetTranslation();
	pos.set_x(pos.x());
	pos.set_y(pos.y() - 8.1f);

	float posY = pos.y();
	pos.set_z(posY);
	pos.set_y(0);

	gef::Vector4 cameraPos = SceneApp::instance->GetCameraLookAt();

	// @TODO: Figure out WHY?
	pos.set_x(pos.x() * 1.35714286);
	pos.set_z(cameraPos.z() + pos.z() * 1.75f);

	return pos;
}

static gef::Vector4 ProjectWorldToScreenSpace(gef::Vector4 v4)
{
	gef::Renderer3D* renderer = SceneApp::instance->GetRenderer3D();
	v4.set_w(1.f);
	v4.Transform(renderer->view_matrix());
	v4.Transform(renderer->projection_matrix());

	float clipSpaceX = v4.x() / v4.w();
	float clipSpaceY = v4.y() / v4.w();

	float xScreen = (clipSpaceX + 1.f) * SceneApp::instance->platform().width() / 2;
	float yScreen = (1 - clipSpaceY) * SceneApp::instance->platform().height() / 2;

	return gef::Vector4(xScreen, yScreen, 0);
}

/// <summary>
/// Finds the angle between 2 Vector4s
/// </summary>
static float FindAngle(const gef::Vector4& a, const gef::Vector4& b)
{
	double dot_product = a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
	double mag_a = std::sqrt(a.x() * a.x() + a.y() * a.y() + a.z() * a.z());
	double mag_b = std::sqrt(b.x() * b.x() + b.y() * b.y() + b.z() * b.z());
	double cos_theta = dot_product / (mag_a * mag_b);
	double theta = std::acos(cos_theta) * 180 / PI;
	return theta;
}

/// <summary>
/// Static implemtation of the Vector4 lerp that is in the Vector4 class
/// </summary>
static gef::Vector4 Lerp(const gef::Vector4& a, const gef::Vector4& b, float alpha)
{
	float x = gef::Lerp(a.x(), b.x(), alpha);
	float y = gef::Lerp(a.y(), b.y(), alpha);
	float z = gef::Lerp(a.z(), b.z(), alpha);

	return gef::Vector4(x, y, z);
}

/// <summary>
/// Enum used to decided on what to apply damage
/// </summary>
namespace ApplyDamageOn
{
	enum Type
	{
		EnemiesOnly,
		PlayerOnly,
		EnemiesAndPlayer
	};
}
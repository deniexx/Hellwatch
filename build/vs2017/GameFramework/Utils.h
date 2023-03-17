#pragma once

#include "maths/Vector2.h"
#include "maths/vector4.h"
#include "maths/matrix44.h"
#include <graphics/renderer_3d.h>
#include <scene_app.h>

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
	pos.set_x(pos.x() * 5);
	pos.set_y(pos.y() * 5 - 25.f);

	float posY = pos.y();
	pos.set_z(posY);
	pos.set_y(0);

	return pos;
}
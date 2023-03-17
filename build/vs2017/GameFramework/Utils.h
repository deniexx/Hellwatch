#pragma once

#define GENERATED_BODY(ParentClass, CurrentClass) typedef ::ParentClass Super; \
									typedef CurrentClass ThisClass;


#define bindFunc(funcName) [this]() { funcName(); }
#define bindFunc_ONEParam(funcName, paramType, paramName) [this](paramType paramName) { funcName(paramName); }
#define bindFunc_TWOParams(funcName, paramType1, paramName1, paramType2, paramName2) [this](paramType1 paramName1, paramType2 paramName2) { funcName(paramName1, paramName2); }
#define bindFunc_THREEParam(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3) { funcName(paramName1, paramName2, paramName3); }
#define bindFunc_FOURParams(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3, paramType4, paramName4) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3, paramType4 paramName4) { funcName(paramName1, paramName2, paramName3, paramName4); }
#define bindFunc_FIVEParams(funcName, paramType1, paramName1, paramType2, paramName2, paramType3, paramName3, paramType4, paramName4, paramType5, paramName5) [this](paramType1 paramName1, paramType2 paramName2, paramType3 paramName3, paramType4 paramName4, paramName5 paramName5) { funcName(paramName1, paramName2, paramName3, paramName4, paramName5); }

template<typename T>
static T Clamp(T expression, T minVal, T maxVal)
{
	if (expression < minVal)
		expression = minVal;
	
	return expression > maxVal ? maxVal : expression;
}
#pragma once

#include "ActorComponents/ActorComponent.h"
#include "GameFramework/Utils.h"
#include "AttributeTypes.h"

#include <vector>

class AttributeComponent : public ActorComponent
{
	GENERATED_BODY(ActorComponent, AttributeComponent);

private:

	mutable std::vector<FAttribute> attributes;

public:

	/// <summary>
	/// Gets the current attribute value
	/// </summary>
	const float GetCurrentAttributeValueByType(HellwatchAttribute::Type type) const;

	/// <summary>
	/// Gets the max attribute value
	/// </summary>
	const float GetMaxAttributeValueByType(HellwatchAttribute::Type type) const;

	/// <summary>
	/// Checks if we have enough of the specified attribute
	/// </summary>
	const bool CheckHasEnoughOfAttributeByType(HellwatchAttribute::Type type, float amountNeeded);

	/// <summary>
	/// Applies an attribute change to the specified attribute, can be positive or negative
	/// </summary>
	void ApplyAttributeChange(HellwatchAttribute::Type type, float delta) const;

	/// <summary>
	/// Initializes attribute component by spec, spec meaning a container of multiple attributes
	/// </summary>
	/// <param name="attributesToInit"></param>
	void InitAttributesBySpec(std::vector<FAttribute>& attributesToInit);

	/// <summary>
	/// Adds an attribute
	/// @NOTE: We can not have 2 attributes of the same type, if that happens, the new attribute will be ignore and we will keep the old one
	/// </summary>
	void AddAttribute(FAttribute& attributeToAdd);
	
};


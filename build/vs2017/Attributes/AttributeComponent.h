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
	/// Sets the attribute current to the new given value
	/// </summary>
	void SetAttributeCurrentValue(HellwatchAttribute::Type type, float newValue);

	/// <summary>
	/// Sets the attribute max value to the new given value
	/// </summary>
	void SetAttributeMaxValue(HellwatchAttribute::Type type, float newValue);

	/// <summary>
	/// Sets the attribute current and max value to the new given value
	/// </summary>
	void SetAttributeCurrentAndMaxValue(HellwatchAttribute::Type, float newValue);

	/// <summary>
	/// Applies an attribute change to the specified attribute, can be positive or negative
	/// </summary>
	void ApplyAttributeChange(HellwatchAttribute::Type type, float delta);

	/// <summary>
	/// Applies an attribute change to the specified attribute's max value, can be positive or negative
	/// </summary>
	void ApplyAttributeMaxValueChange(HellwatchAttribute::Type type, float delta);

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

	/// <summary>
	/// Applies a multiplier to an attribute
	/// </summary>
	void ApplyAttributeMultipliyer(HellwatchAttribute::Type type, float multipliyer);

private:

	/// <summary>
	/// Updates health and mana based on attributes changed
	/// </summary>
	void OnAttributeChanged(HellwatchAttribute::Type type, float oldAmount, float newAmount);
};


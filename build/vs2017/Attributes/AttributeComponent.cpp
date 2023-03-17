#include "AttributeComponent.h"

#include <algorithm>

const float AttributeComponent::GetCurrentAttributeValueByType(HellwatchAttribute::Attribute type) const
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.currentAmount;
	}

	return 0.0f;
}

const float AttributeComponent::GetMaxAttributeValueByType(HellwatchAttribute::Attribute type) const
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.maxAmount;
	}

	return 0.0f;
}

const bool AttributeComponent::CheckHasEnoughOfAttributeByType(HellwatchAttribute::Attribute type, float amountNeeded)
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.currentAmount > amountNeeded;
	}

	return false;
}

void AttributeComponent::ApplyAttributeChange(HellwatchAttribute::Attribute type, float delta) const
{
	for (auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
		{
			attribute.currentAmount += delta;
			if (attribute.bClampedToZero)
				attribute.currentAmount = Clamp<float>(attribute.currentAmount, 0, attribute.maxAmount);
		}
	}
}

void AttributeComponent::InitAttributesBySpec(std::vector<FAttribute>& attributesToInit)
{
	attributes.clear();

	for (auto att : attributesToInit)
		attributes.push_back(att);
}

void AttributeComponent::AddAttribute(FAttribute& attributeToAdd)
{
	for (auto& att : attributes)
		if (att.attributeType == attributeToAdd.attributeType)
			return;

	attributes.push_back(attributeToAdd);
}

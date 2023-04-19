#include "AttributeComponent.h"

#include <algorithm>

const float AttributeComponent::GetCurrentAttributeValueByType(HellwatchAttribute::Type type) const
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.currentAmount;
	}

	return 0.0f;
}

const float AttributeComponent::GetMaxAttributeValueByType(HellwatchAttribute::Type type) const
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.maxAmount;
	}

	return 0.0f;
}

const bool AttributeComponent::CheckHasEnoughOfAttributeByType(HellwatchAttribute::Type type, float amountNeeded)
{
	for (const auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
			return attribute.currentAmount > amountNeeded;
	}

	return false;
}

void AttributeComponent::SetAttributeCurrentValue(HellwatchAttribute::Type type, float newValue)
{
	for (auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
		{
			attribute.currentAmount = newValue;
			break;
		}
	}
}

void AttributeComponent::SetAttributeMaxValue(HellwatchAttribute::Type type, float newValue)
{
	for (auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
		{
			attribute.maxAmount = newValue;
			break;
		}
	}
}

void AttributeComponent::SetAttributeCurrentAndMaxValue(HellwatchAttribute::Type type, float newValue)
{
	SetAttributeMaxValue(type, newValue);
	SetAttributeCurrentValue(type, newValue);
}

void AttributeComponent::ApplyAttributeChange(HellwatchAttribute::Type type, float delta)
{
	for (auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
		{
			float oldAmount = attribute.currentAmount;
			attribute.currentAmount += delta;
			if (attribute.bClampedToZero)
				attribute.currentAmount = Clamp<float>(attribute.currentAmount, 0, attribute.maxAmount);

			OnAttributeChanged(type, oldAmount, attribute.currentAmount);
			break;
		}
	}
}

void AttributeComponent::ApplyAttributeMaxValueChange(HellwatchAttribute::Type type, float delta)
{
	for (auto& attribute : attributes)
	{
		if (attribute.attributeType == type)
		{
			attribute.maxAmount += delta;
			break;
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

void AttributeComponent::ApplyAttributeMultiplier(HellwatchAttribute::Type type, float multiplier)
{
	float maxValue = GetMaxAttributeValueByType(type);
	float currentValue = GetCurrentAttributeValueByType(type);

	SetAttributeMaxValue(type, maxValue * multiplier);
	SetAttributeCurrentValue(type, currentValue * multiplier);
}

void AttributeComponent::OnAttributeChanged(HellwatchAttribute::Type type, float oldAmount, float newAmount)
{
	if (type == HellwatchAttribute::Vitality)
	{
		ApplyAttributeMultiplier(HellwatchAttribute::Health, ((newAmount - oldAmount) * 0.1f) + 1);
	}
	else if (type == HellwatchAttribute::Energy)
	{
		ApplyAttributeMultiplier(HellwatchAttribute::Energy, ((newAmount - oldAmount) * 0.1f) + 1);
	}
}

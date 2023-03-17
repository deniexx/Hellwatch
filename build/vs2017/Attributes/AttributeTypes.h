#pragma once

#include <string>

/// <summary>
/// Attribute Type
/// </summary>
namespace HellwatchAttribute
{
	enum Type
	{
		Health = 0,
		Mana,

		Vitality,
		Wisdom,
		Toughness,
		Energy,

		None
	};
}


/// <summary>
/// Attribute representation
/// </summary>
struct FAttribute
{
	HellwatchAttribute::Type attributeType;
	float maxAmount = 100.f;
	float currentAmount = 0.f;

	bool bClampedToZero = true;
};

/// <summary>
/// Helper function to get attribute name in string format
/// </summary>
static std::string AttributeToString(HellwatchAttribute::Type type)
{
	switch (type)
	{
		case HellwatchAttribute::Health:
			return "Health";
		case HellwatchAttribute::Mana:
			return "Mana";
		case HellwatchAttribute::Vitality:
			return "Vitality";
		case HellwatchAttribute::Wisdom:
			return "Wisdom";
		case HellwatchAttribute::Toughness:
			return "Toughness";
		case HellwatchAttribute::Energy:
			return "Energy";
		case HellwatchAttribute::None:
			return "None";
		default:
			return "";
	}
}

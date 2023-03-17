#pragma once

#include <string>

namespace HellwatchAttribute
{
	enum Attribute
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

struct FAttribute
{
	HellwatchAttribute::Attribute attributeType;
	float maxAmount = 100.f;
	float currentAmount = 0.f;

	bool bClampedToZero = true;
};

static std::string AttributeToString(HellwatchAttribute::Attribute type)
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

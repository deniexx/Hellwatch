#pragma once

#include "ActorComponents/ActorComponent.h"
#include "GameFramework/Utils.h"
#include "AttributeTypes.h"

#include <vector>

class AttributeComponent : public ActorComponent
{
	GENERATED_BODY(ActorComponent, AttributeComponent);

public:

	

private:

	mutable std::vector<FAttribute> attributes;

public:

	const float GetCurrentAttributeValueByType(HellwatchAttribute::Attribute type) const;
	const float GetMaxAttributeValueByType(HellwatchAttribute::Attribute type) const;
	const bool CheckHasEnoughOfAttributeByType(HellwatchAttribute::Attribute type, float amountNeeded);

	void ApplyAttributeChange(HellwatchAttribute::Attribute type, float delta) const;
	void InitAttributesBySpec(std::vector<FAttribute>& attributesToInit);
	void AddAttribute(FAttribute& attributeToAdd);
	
};


#include "stdafx.h"
#include "AIComponent.h"

AIComponent::AIComponent(Entity& entity, Entity& self)
	: self(self), entity(entity)
{
}

AIComponent::~AIComponent()
{
}

void AIComponent::update(const float& dt)
{
}
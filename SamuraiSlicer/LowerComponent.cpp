#include "LowerComponent.h"
#include "GameObject.h"


LowerComponent::LowerComponent()
{
	speed = 4.5;
}


LowerComponent::~LowerComponent()
{
}

void LowerComponent::update(float elapsedTime)
{
	gameObject->position.x -= elapsedTime * speed;
}
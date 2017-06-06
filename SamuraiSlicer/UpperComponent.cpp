#include "UpperComponent.h"
#include "GameObject.h"


UpperComponent::UpperComponent()
{
	speed = 4.5;
}


UpperComponent::~UpperComponent()
{
}

void UpperComponent::update(float elapsedTime)
{
	gameObject->position.x += elapsedTime * speed;
}
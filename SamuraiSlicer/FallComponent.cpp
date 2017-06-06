#include "FallComponent.h"
#include "GameObject.h"


FallComponent::FallComponent()
{
	speed = 4.5;
}


FallComponent::~FallComponent()
{
}

FallComponent::FallComponent(float speedVar)
{
	speed = speedVar;
}

void FallComponent::update(float elapsedTime)
{
	gameObject->position.y -= elapsedTime * speed;
}

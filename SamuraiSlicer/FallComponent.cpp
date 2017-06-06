#include "FallComponent.h"
#include "GameObject.h"


FallComponent::FallComponent()
{
	speed = 4.5;
}


FallComponent::~FallComponent()
{
}

void FallComponent::update(float elapsedTime)
{
	gameObject->position.y -= elapsedTime * speed;
}

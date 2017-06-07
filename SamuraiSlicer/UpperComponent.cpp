#include "UpperComponent.h"
#include "GameObject.h"


bool chosenDir = false;

UpperComponent::UpperComponent()
{
	speed = 6;
}


UpperComponent::~UpperComponent()
{
}

void UpperComponent::update(float elapsedTime)
{
	gameObject->position.x += elapsedTime * speed;
	gameObject->position.y += elapsedTime * speed;
	/*int random;
	if (chosenDir == false) {
		random = rand() % 2;
		chosenDir = true;
	}

	if (random == 0) {
		gameObject->position.y += (elapsedTime * speed);
	}else{
		gameObject->position.y -= (elapsedTime * speed);
	}*/
}
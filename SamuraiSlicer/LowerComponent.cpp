#include "LowerComponent.h"
#include "GameObject.h"

bool chosenDirection = false;


LowerComponent::LowerComponent()
{
	speed = 6;
}


LowerComponent::~LowerComponent()
{
}

void LowerComponent::update(float elapsedTime)
{
	gameObject->position.x -= elapsedTime * speed;
	gameObject->position.y -= elapsedTime * speed;
	
	/*int random;
	if (chosenDirection == false) {
		random = rand() % 2;
		chosenDirection = true;
	}

	if (random == 0) {
		gameObject->position.y -= (elapsedTime * speed);
	}
	else {
		gameObject->position.y += (elapsedTime * speed);
	}*/
}
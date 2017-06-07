#include "LowerComponent.h"
#include "GameObject.h"
#include <GL/freeglut.h>

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
	float random = ((glutGet(GLUT_ELAPSED_TIME) % 20) / 10) - 1;

	gameObject->position.x -= elapsedTime * speed;
	gameObject->position.y -= elapsedTime * (speed * random);
	
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
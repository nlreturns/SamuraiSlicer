#pragma once
#include <GL/freeglut.h>
#include "GameObject.h"

struct plaine {
	double topX1;
	double topX2;
	double downX1;
	double downX2;
	double topY1;
	double topY2;
	double downY1;
	double downY2;
};

plaine SwordPlaine;

void DrawSwordPlaine(void);
bool DetectCollision(GameObject object);

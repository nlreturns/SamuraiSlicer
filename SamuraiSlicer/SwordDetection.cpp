#include "SwordDetection.h"

// Returns true if the GameObject colides with the sword plaine
bool DetectCollision(GameObject object) {
	return (
		((object.position.x >= SwordPlaine.topX1) && (object.position.x <= SwordPlaine.topX2)) && 
		((object.position.y <= SwordPlaine.topY1) && (object.position.y >= SwordPlaine.topY2))
		);
}

// Draws a plaine between 2 frames
void DrawSwordPlaine() {

	SwordPlaine = {
		0,
		1,
		0,
		1,
		0,
		0,
		1,
		1
	};

	glBegin(GL_QUADS);
	glColor3f(0, 255, 0);
	glVertex3f(SwordPlaine.topX1, SwordPlaine.topY1, 0);
	glVertex3f(SwordPlaine.topX2, SwordPlaine.topY2, 0);
	glVertex3f(SwordPlaine.downX2, SwordPlaine.downY2, 0);
	glVertex3f(SwordPlaine.downX1, SwordPlaine.downY1, 0);
	glEnd();

}
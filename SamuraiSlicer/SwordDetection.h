#pragma once
#include <GL/freeglut.h>
#include "GameObject.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

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


void DrawSwordPlaine(int fx, int fy, int lx, int ly);
bool DetectCollision(GameObject object);
int readCam(void);

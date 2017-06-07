#pragma once

#include "GameObject.h"


void reshape(int w, int h);

void playSounds(int nr);

void playMusic(int nr);

void printScore(int s);

void keyboard(unsigned char key, int x, int  y);

void loadBackground();

void loadStartscreen();

void loadVictoryscreen();

void loadDefeatscreen();

void startMenu();

void printTime();

void initFruit();

void camInit();

void init();

void display();

void CamLoop();

void GameObjectCollision(GameObject *o);

void idle();

void mouseButton(int button, int state, int x, int y);





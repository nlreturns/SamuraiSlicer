#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include <math.h>
#include "GameObject.h"
#include "CubeComponent.h"
#include "ObjectComponent.h"
#include "SpinComponent.h"
#include "FallComponent.h"

int height = 800;
int width = 1200;

std::list<GameObject*> objects;

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int  y)
{
	if (key == 27)
		exit(0);
}

GLuint background;
void loadBackground() {
	glGenTextures(1, &background);
	glBindTexture(GL_TEXTURE_2D, background);

	int width, height, depth;
	unsigned char* data = stbi_load("images/background.png", &width, &height, &depth, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}



GLuint startscreen;
void loadStartscreen() {
	glGenTextures(1, &background);
	glBindTexture(GL_TEXTURE_2D, background);

	int width, height, depth;
	unsigned char* data = stbi_load("images/startscreen.png", &width, &height, &depth, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void initFruit() {

	GameObject* apple = new GameObject();
	apple->addComponent(new ObjectComponent("models/appeltje/appeltje.obj"));
	apple->addComponent(new SpinComponent(40.0f));
	apple->addComponent(new FallComponent());
	apple->position = Vec3f(0.0f, 15.0f, 0.0f);
	objects.push_back(apple);

	GameObject* banana = new GameObject();
	banana->addComponent(new ObjectComponent("models/banaan/banaan.obj"));
	banana->addComponent(new SpinComponent(40.0f));
	banana->addComponent(new FallComponent());
	banana->position = Vec3f(3.0f, 19.0f, 0.0f);
	objects.push_back(banana);
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	initFruit();

	loadStartscreen();
}

void display()
{
	loadBackground();
	glClearColor(0.4f, 0.4f, 0.4f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background);
	glColor4f(1, 1, 1, 1);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, height, 0);
	glTexCoord2f(1, 1); glVertex3f(width, height, 0);
	glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)width / (float)height, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 7,
			  0, 0, 0,
			  0, 1, 0);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto &o : objects)
		o->draw();


	glutSwapBuffers();
}

int lastTime = 0;
void idle()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	for (auto &o : objects)
		o->update(deltaTime);

	glutPostRedisplay();
}


void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		glutDisplayFunc(display);
		glutIdleFunc(idle);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
	}
}

void startMenu() {
	glClearColor(0.4f, 0.4f, 0.4f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background);
	glColor4f(1, 1, 1, 1);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, height, 0);
	glTexCoord2f(1, 1); glVertex3f(width, height, 0);
	glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)width / (float)height, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 7,
		0, 0, 0,
		0, 1, 0);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glClear(GL_DEPTH_BUFFER_BIT);



	glutSwapBuffers();
}


int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInit(&argc, argv);
	glutCreateWindow("Samurai Slicer");
	glutDisplayFunc(startMenu);
	glutMouseFunc(mouseButton);
	init();

	glutMainLoop();

	return 0;
}
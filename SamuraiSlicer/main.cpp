#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include <math.h>
#include "GameObject.h"
#include "CubeComponent.h"
#include "ObjectComponent.h"
#include "SpinComponent.h"
#include "SwordDetection.h"
#include "FallComponent.h"

int height = 800;
int width = 1200;
int timeElapsed = 0;
int spawnTime = 3000;

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


void initFruit() {

	for (int i = 0; i < 500; i ++) {
		GameObject* fruit = new GameObject();

		int random = rand() % 3;	
		if (random == 0) 
			fruit->addComponent(ObjectComponent::build("models/citroen/citroenBovenkant.obj"));
		else if (random == 1)
			fruit->addComponent(ObjectComponent::build("models/citroen/citroenOnderkant.obj"));
		else if(random == 2)
			fruit->addComponent(ObjectComponent::build("models/citroen/citroen.obj"));
		fruit->addComponent(new SpinComponent(rand()%40+20));
		fruit->addComponent(new FallComponent());
		fruit->position = Vec3f((rand()%200-100)/10, i+10, 0.0f);

		objects.push_back(fruit);
	}

}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	initFruit();

	loadBackground();
}

void display()
{
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

	//DrawSwordPlaine();

	glutSwapBuffers();
}

int lastTime = 0;
bool firstTime = true;
void idle()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (firstTime) {
		firstTime = false;
		lastTime = currentTime;
	}
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	for (auto &o : objects)
		o->update(deltaTime);

	for (GameObject* o : objects) {
		if (DetectCollision(*o)) 
			objects.remove(o);
	}

	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInit(&argc, argv);
	glutCreateWindow("Samurai Slicer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	init();
//	readCam();
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
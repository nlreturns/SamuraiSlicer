#include <GL/freeglut.h>
#include <math.h>
#include "GameObject.h"
#include "CubeComponent.h"
#include "SpinComponent.h"
#include "SwordDetection.h"

int height = 800;
int width = 1200;


std::list<GameObject*> objects;

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int  y)
{
	if (key == 27)
		exit(0);
}


void init()
{
	glEnable(GL_DEPTH_TEST);
	
	for(int i = -10; i < 10; i++)
	{
		GameObject* o = new GameObject();
		o->addComponent(new CubeComponent(0.5));
		o->addComponent(new SpinComponent((i+20)*2));
		o->position = Vec3f(i, 0, 0);
		objects.push_back(o);
	}

}

void display()
{
	glClearColor(0.9f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, width / (float)height, 0.1f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -5, 5,
			  0, 0, 0,
			  0, 1, 0);


//	for (auto &o : objects)
//		o->draw();

	DrawSwordPlaine();

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


int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInit(&argc, argv);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//opengl init
	init();
	readCam();

	glutMainLoop();

	return 0;
}
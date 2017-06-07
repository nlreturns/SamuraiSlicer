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
#include <irrKlang.h>
#include "Sounds.h"

int height = 800;
int width = 1200;
int timeElapsed = 0;
int spawnTime = 3000;
int score = 0;
bool isStarted = false;

int fx, fy;
int lx, ly;

std::list<GameObject*> objects;
irrklang::ISoundEngine* engine;


cv::VideoCapture cap(0);
cv::Mat frame, nonFlipped, hsvFrame, rgbFrame;
cv::Mat redChannel[3], saturationChannel[3], redValue, satValue, sword;

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}

void playSounds(int nr)
{
	if (nr == 0)
		engine->play2D("Sounds/click.mp3", false);
	if (nr == 1)
		engine->play2D("Sounds/SlicingSound.mp3", false);
	if (nr == 2)
		engine->play2D("Sounds/SamuraiSlicer.mp3", false);
	if (nr == 3)
		engine->play2D("Sounds/explosion.wav", false);
}

void playMusic(int nr)
{
	engine->stopAllSounds();

	if (nr == 0)
		engine->play2D("Sounds/OpeningShogun.mp3", true);
	if (nr == 1)
		engine->play2D("Sounds/MainMoyuru.mp3", true);
	if (nr == 2)
		engine->play2D("Sounds/NinjaBattle.mp3", true);
	if (nr == 3)
		engine->play2D("Sounds/OutroYuunagi.mp3", true);
	if (nr == 4)
		engine->play2D("Sounds/MysteryShadowNinja.mp3", true);
}

void printScore(int s)
{
	char score[32];
	_itoa_s(s, score, 10);
	int lenght = floor(log10(abs(s))) + 1;
	glRasterPos3f(-3.1f, 1.9f, 4.9f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	if (s == 0) {glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');}
	else {for (int i = 0; i < lenght; i++)
		{glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);}}
}

void keyboard(unsigned char key, int x, int  y)
{
	if (key == 27)
		exit(0);
	if (key == 49)
		playMusic(0);
	if (key == 50)
		playMusic(1);
	if (key == 51)
		playMusic(2);
	if (key == 52)
		playMusic(3);
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
	for (int i = 0; i < 520; i ++) {
		GameObject* fruit = new GameObject();

		int random = rand() % 4;	
		if (random == 0)
			fruit->addComponent(ObjectComponent::build("models/appeltje/appeltje.obj"));
		else if (random == 1)
			fruit->addComponent(ObjectComponent::build("models/banaan/banaan.obj"));
		else if (random == 2)
			fruit->addComponent(ObjectComponent::build("models/citroen/citroen.obj"));
		else if (random == 3)
			fruit->addComponent(ObjectComponent::build("models/bom/bom5.obj"));
		fruit->addComponent(new SpinComponent(rand()%40+20));
		fruit->addComponent(new FallComponent());
		fruit->position = Vec3f((rand()%200-100)/10, i+10, 0.0f);

		objects.push_back(fruit);
	}
}

void camInit() 
{
	cv::namedWindow("SamuraiSlicer", CV_WINDOW_AUTOSIZE);
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	engine = irrklang::createIrrKlangDevice();
	playMusic(0);
	camInit();

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

	printScore(score); // Print score to screen
	for (auto &o : objects)
		o->draw();

	DrawSwordPlaine(fx, fy, lx, ly);

	glutSwapBuffers();
}

void CamLoop()
{
	// Lees een nieuw frame
	bool bSuccess = cap.read(nonFlipped);

	// Controlleer of het frame goed gelezen is.
	if (!bSuccess)
	{
		std::cout << "Cannot read a frame from video stream" << std::endl;
	}
	//flips frame
	flip(nonFlipped, frame, 1);

	//get saturation channel
	cvtColor(frame, hsvFrame, CV_RGB2HSV);
	split(hsvFrame, saturationChannel);

	// get red channel
	split(frame, redChannel);

	//thresholds channels
	threshold(redChannel[2], redValue, 220, 255, CV_THRESH_BINARY);
	threshold(saturationChannel[1], satValue, 160, 255, CV_THRESH_BINARY);

	//multiplies thresholded channels
	multiply(redValue, satValue, sword, 1.0, -1);

	imshow("SamuraiSlicer", sword);

	findFirstPixel(sword, &fx, &fy);
	findLastPixel(sword, &lx, &ly);

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

	CamLoop();

	for (auto &o : objects)
		o->update(deltaTime);

	for (GameObject* o : objects) {
		if (DetectCollision(*o)) {
			objects.remove(o);
			score++;
		}
	}

	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON && isStarted == false) {
		glutDisplayFunc(display);
		glutIdleFunc(idle);
		loadBackground();
		initFruit();
		isStarted = true;
	}

	glutPostRedisplay();
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
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
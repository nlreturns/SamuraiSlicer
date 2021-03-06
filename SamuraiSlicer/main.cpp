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
#include "UpperComponent.h"
#include "LowerComponent.h"
#include <irrKlang.h>
#include "Sounds.h"
#include "Main.h"

int height = 800;
int width = 1200;
int timeElapsed = 0;
int spawnTime = 3000;
int score = 0;
int time, startTime, length;
bool isStarted = false;
bool soundUit = false;

int fx, fy;
int lx, ly;

std::list<GameObject*> objects;
irrklang::ISoundEngine* engine;

cv::VideoCapture cap(1);
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
	if (!soundUit) {
		if (nr == 0)
			engine->play2D("Sounds/click.mp3", false);
		if (nr == 1)
			engine->play2D("Sounds/Slicing1.mp3", false);
		if (nr == 2)
			engine->play2D("Sounds/SamuraiSlicer.mp3", false);
		if (nr == 3)
			engine->play2D("Sounds/explosion.wav", false);
		if (nr == 4)
			engine->play2D("Sounds/Slicing2.mp3", false);
		if (nr == 5)
			engine->play2D("Sounds/Slicing3.mp3", false);
		if (nr == 6)
			engine->play2D("Sounds/Slicing4.mp3", false);
		if (nr == 7)
			engine->play2D("Sounds/Slicing5.mp3", false);
	}

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
	glEnable(GL_COLOR);
	glDisable(GL_LIGHTING);

	char score[32];
	_itoa_s(s, score, 10);
	length = floor(log10(abs(s))) + 1;
	glColor3f(1, 1, 1);
	glRasterPos3f(-3.1f, 1.9f, 4.9f);
	glEnable(GL_COLOR);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	if (s == 0) {glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');}
	else {for (int i = 0; i < length; i++)
		{glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);}}
	glDisable(GL_LIGHTING);
}

/**
* print time left to play the game,
* when the timer hits 0 call method @TODO
*/
void printTime() {
	char timeLeft[32];
	int substract = glutGet(GLUT_ELAPSED_TIME) - startTime;
	time = (120000 - substract) / 1000;
	_itoa_s(time, timeLeft, 10);

	glColor3f(1, 1, 1);
	glRasterPos3f(2.0f, 1.9f, 4.9f);
	std::string timeleft = "Time left: ";
	for (char& c : timeleft) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	if (time == 0) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
		// time is 0, call a method here.
		loadVictoryscreen();
		glutDisplayFunc(victoryMenu);
	}
	else if (time < 60) { // time is lower than 60 seconds, print easy algorithm
		length = floor(log10(abs(time))) + 1;
		for (int i = 0; i < length; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timeLeft[i]);
	}
	else { // time is higher than 60 seconds, print harder algorithm
		char minutes[32];
		_itoa_s(time / 60, minutes, 10);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, minutes[0]);

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
		//seconds
		_itoa_s(time % 60, timeLeft, 10);
		length = floor(log10(abs(time % 60))) + 1;
		for (int i = 0; i < length; i++) {
			if (length < 2)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timeLeft[i]);
		}
		if (time % 60 == 0) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
		}
	}
}

void printVictoryScore(int s)
{
	char score[32];
	_itoa_s(s, score, 10);
	length = floor(log10(abs(s))) + 1;
	glColor3f(1, 1, 1);
	glRasterPos3f(-0.9f, -0.88f, 4.9f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	if (s == 0) { glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'); }
	else {
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);
		}
	}
}

void printDefeatScore(int s) {
	char score[32];
	_itoa_s(s, score, 10);
	length = floor(log10(abs(s))) + 1;
	glColor3f(1, 1, 1);
	glRasterPos3f(-0.5f, 0.0f, 4.9f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	if (s == 0) { glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'); }
	else {
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);
		}
	}
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
	unsigned char* data = stbi_load("images/samuraislicer.jpg", &width, &height, &depth, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void loadVictoryscreen() {

	isStarted = false;

	glGenTextures(1, &background);
	glBindTexture(GL_TEXTURE_2D, background);

	int width, height, depth;
	unsigned char* data = stbi_load("images/Victoryscreen.png", &width, &height, &depth, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void loadDefeatscreen() {

	isStarted = false;

	glGenTextures(1, &background);
	glBindTexture(GL_TEXTURE_2D, background);

	int width, height, depth;
	unsigned char* data = stbi_load("images/Defeatscreen.png", &width, &height, &depth, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

void victoryMenu() {
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
	printVictoryScore(score);
	glutSwapBuffers();
}

void defeatMenu() {
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
	printDefeatScore(score);
	glutSwapBuffers();
}


void initFruit() {
	for (int i = 0; i < 520; i ++) {
		GameObject* fruit = new GameObject();

		int random = rand() % 16;	
		if (random <= 4) 
			fruit->addComponent(ObjectComponent::build("models/appeltje/appeltje.obj"));
		else if (random >= 5 && random <= 9)
			fruit->addComponent(ObjectComponent::build("models/banaan/banaan.obj"));
		else if(random >= 10 && random <= 14)
			fruit->addComponent(ObjectComponent::build("models/citroen/citroen.obj"));
		else {
			fruit->addComponent(ObjectComponent::build("models/bom/grayBom.obj"));
		}

		fruit->index = random;
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
	//camInit();

	loadStartscreen();
	
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
	glEnable(GL_COLOR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_LIGHTING);
	printScore(score); // Print score to screen
	printTime();
	glEnable(GL_LIGHTING);
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

	//imshow("SamuraiSlicer", sword);

	findFirstPixel(sword, &fx, &fy);
	findLastPixel(sword, &lx, &ly);

}


void GameObjectCollision(GameObject *o) {
	if (DetectCollision(*o) && o->collision) {
		GameObject* upper = new GameObject();
		GameObject* lower = new GameObject();

		if (!soundUit)
			score++;

		glDisable(GL_TEXTURE_2D);

		if (o->index <= 4) {
			upper->addComponent(ObjectComponent::build("models/appeltje/appeltjeBovenkant.obj"));
			lower->addComponent(ObjectComponent::build("models/appeltje/appeltjeOnderkant.obj"));
		}		
		else if (o->index >=  5 && o->index <= 9) {
			upper->addComponent(ObjectComponent::build("models/banaan/banaanBovenkant.obj"));
			lower->addComponent(ObjectComponent::build("models/banaan/banaanOnderkant.obj"));
		}
		else if (o->index >= 10 && o->index <= 14) {
			upper->addComponent(ObjectComponent::build("models/citroen/citroenBovenkant.obj"));
			lower->addComponent(ObjectComponent::build("models/citroen/citroenOnderkant.obj"));
		}
		else if (o->index == 15) {
			playSounds(3);
			loadDefeatscreen();
			glutDisplayFunc(defeatMenu);
			soundUit = true;		
		}

		glDisable(GL_TEXTURE_2D);

		upper->addComponent(new SpinComponent(rand() % 40 + 20));
		lower->addComponent(new SpinComponent(rand() % 40 + 20));
		upper->position = o->position;
		lower->position = o->position;

		upper->addComponent(new UpperComponent());
		lower->addComponent(new LowerComponent());

		upper->collision = false;
		lower->collision = false;

		objects.remove(o);
		objects.push_back(upper);
		objects.push_back(lower);


		int random = rand() % 5;
		if (random == 0)
			playSounds(1);
		else if (random == 1)
			playSounds(4);
		else if (random == 2)
			playSounds(5);
		else if (random == 3)
			playSounds(6);
		else
			playSounds(7);
	}
	else if (!o->collision) {
		o->count++;
		if (o->count > 10) {
			objects.remove(o);
		}
	}
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

	try {
		for (GameObject* o : objects) 
			GameObjectCollision(o);
	}
	catch (std::exception e) {
		std::cout << "Uknown Expcetion" << std::endl;
	}

	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON && isStarted == false) {
		glutDisplayFunc(display);
		glutIdleFunc(idle);
		playSounds(2);
		loadBackground();
		objects.clear();
		initFruit();
		playMusic(2);
		playSounds(2);
		isStarted = true;
		glEnable(GL_COLOR);
		glColor3f(1.0f, 0, 0);
		score = 0;
		soundUit = false;
		startTime = glutGet(GLUT_ELAPSED_TIME);
	}

	if (button == GLUT_LEFT_BUTTON) {
		playSounds(0);
	}
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

	glutMainLoop();

	return 0;
}
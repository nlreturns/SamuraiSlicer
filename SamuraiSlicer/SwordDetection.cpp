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

int readCam()
{
	VideoCapture cap(1);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	// Window maken waarin de beelden "live" getoond worden
	namedWindow("SamuraiSlicer", CV_WINDOW_AUTOSIZE);
	namedWindow("Channel 1", CV_WINDOW_AUTOSIZE);
	namedWindow("Channel 2", CV_WINDOW_AUTOSIZE);

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame, nonFlipped, hsvFrame, grayFrame;
	Mat upperRed, lowerRed, sword;
	//testcode
	namedWindow("SWORD", CV_WINDOW_AUTOSIZE);

	while (1)
	{

		// Lees een nieuw frame
		bool bSuccess = cap.read(nonFlipped);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		//flips frame
		flip(nonFlipped, frame, 1);

		//get grayscale and hls from original
		cvtColor(frame, hsvFrame, CV_BGR2HLS);
		cvtColor(frame, grayFrame, CV_BGR2GRAY);

		//threshold color red
		inRange(hsvFrame, Scalar(170, 160, 100), Scalar(179, 255, 255), upperRed);
		inRange(hsvFrame, Scalar(0, 160, 100), Scalar(10, 255, 255), lowerRed);

		//testcode

		imshow("Channel 1", upperRed);
		imshow("Channel 2", lowerRed);



		//imshow("SWORD", sword);

		imshow("SamuraiSlicer", frame);
		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;

}
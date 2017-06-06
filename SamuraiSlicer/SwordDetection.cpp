#include "SwordDetection.h"

//func def
int readCam();
void findFirstPixel(cv::Mat, int *, int *);
void findLastPixel(cv::Mat, int *, int *);

plaine SwordPlaine;

// Returns true if the GameObject colides with the sword plaine
bool DetectCollision(GameObject object) {
	return (((object.position.x >= SwordPlaine.topX1) && (object.position.x <= SwordPlaine.topX2)) && 
		((object.position.y >= SwordPlaine.topY1) && (object.position.y <= SwordPlaine.downY1)));
}

// Draws a plaine between 2 frames
void DrawSwordPlaine() {

	SwordPlaine = {
		0,
		5,
		0,
		5,
		0,
		0,
		5,
		5
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
	cv::VideoCapture cap(1);

	// tries to open 1 of the 2 camera's
	if (!cap.isOpened())
	{
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			cout << "Can't open cam exiting" << endl;
			exit(0);
		}
	}

	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::cout << "Frame size : " << dWidth << " x " << dHeight << std::endl;

	// Window maken waarin de beelden "live" getoond worden
	cv::namedWindow("SamuraiSlicer", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Sword 1", CV_WINDOW_AUTOSIZE);

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	cv::Mat frame, nonFlipped, hsvFrame, rgbFrame;
	cv::Mat redChannel[3], saturationChannel[3], redValue, satValue,sword;

	while (1)
	{

		// Lees een nieuw frame
		bool bSuccess = cap.read(nonFlipped);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			std::cout << "Cannot read a frame from video stream" << std::endl;
			break;
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

		//testcode show image
		imshow("Sword", sword);
		imshow("SamuraiSlicer", frame);

		int fx, fy;
		int lx, ly;
		findFirstPixel(sword, &x, &y);
		findLastPixel(sword, &x, &y);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (cv::waitKey(1) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
	return 0;

}

void findFirstPixel(cv::Mat m, int * fY, int * fX)
{
	bool found = false;
	for (int y = 0; y < m.rows; y++)
	{
		for (int x = 0; x < m.cols; x++)
		{
			int value = m.at<unsigned char>(y, x);
			//cout << value << endl;

			if (value != 0)
			{
				cout << " First: y = " << y << " X = " << x << "Value = " << endl;
				found = true;
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}

void findLastPixel(cv::Mat m, int * lY, int * lX)
{
	bool found = false;
	for (int y = m.rows - 1; y >= 0; y--)
	{
		for (int x = m.cols - 1; x >= 0; x--)
		{
			int value = m.at<unsigned char>(y, x);
			//cout << value << endl;

			if (value != 0)
			{
				cout << "Last: y = " << y << " X = " << x << "Value = " << endl;
				found = true;
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}
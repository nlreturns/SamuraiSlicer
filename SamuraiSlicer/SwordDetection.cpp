#include "SwordDetection.h"

//func def
int readCam();
void findFirstPixel(cv::Mat, int *, int *);
void findLastPixel(cv::Mat, int *, int *);
int previousX1 = 0, previousY1 = 0, previousX2 = 0, previousY2 = 0;
float Xschaal = 640 / 19;
float Yschaal = 480 / 14;

plaine SwordPlaine;

// Returns true if the GameObject colides with the sword plaine
bool DetectCollision(GameObject object) {

	//std::cout << "x1: " << SwordPlaine.topX1 << " x2: " << SwordPlaine.topX2 << "y1: " << SwordPlaine.topY1 << "y2: " << SwordPlaine.downY2 << std::endl;
	//std::cout << "Fruit x: " << object.position.x << " y: " << object.position.y << "lx: " << std::endl;

	if (((object.position.x >= SwordPlaine.topX1) && (object.position.x <= SwordPlaine.topX2)) &&
		((object.position.y >= SwordPlaine.topY1) && (object.position.y <= SwordPlaine.downY1)))
		return true;
	if (((object.position.x <= SwordPlaine.topX1) && (object.position.x >= SwordPlaine.topX2)) &&
		((object.position.y >= SwordPlaine.topY1) && (object.position.y <= SwordPlaine.downY1)))
		return true;
	if (((object.position.x >= SwordPlaine.topX1) && (object.position.x <= SwordPlaine.topX2)) &&
		((object.position.y <= SwordPlaine.topY1) && (object.position.y >= SwordPlaine.downY1)))
		return true;
	if (((object.position.x <= SwordPlaine.topX1) && (object.position.x >= SwordPlaine.topX2)) &&
		((object.position.y <= SwordPlaine.topY1) && (object.position.y >= SwordPlaine.downY1)))
		return true;
	
	return false; 
}

// Draws a plaine between 2 frames
void DrawSwordPlaine(int fx, int fy, int lx, int ly) {

	SwordPlaine = {
		previousX1 / Xschaal - 9.5f,
		fx / Xschaal - 9.5f,
		previousX2 / Xschaal - 9.5f,
		lx / Xschaal - 9.5f,
		(previousY1 / Yschaal - 7.0f)*-1,
		(fy / Yschaal - 7.0f)*-1,
		(previousY2 / Yschaal - 7.0f)*-1,
		(ly / Yschaal / 7.0f)*-1
	};

	//SwordPlaine = {
	//	fx / 19 -9.5f,
	//	lx / 19 - 9.5f,
	//	fx / 19 - 9.5f,
	//	lx / 19 - 9.5f,
	//	fy / 14 - 7.0f,
	//	fy / 14 - 7.0f,
	//	ly / 14 - 7.0f,
	//	ly / 14 / 7.0f
	//};

	previousX1 = fx;
	previousX2 = lx;
	previousY1 = fy;
	previousY2 = ly;

	glBegin(GL_QUADS);
	glColor3f(0, 255, 0);
	glVertex3f(SwordPlaine.topX1, SwordPlaine.topY1, 1);
	glVertex3f(SwordPlaine.topX2, SwordPlaine.topY2, 1);
	glVertex3f(SwordPlaine.downX2, SwordPlaine.downY2, 1);
	glVertex3f(SwordPlaine.downX1, SwordPlaine.downY1, 1);
	glEnd();

}

void initCam() 
{
	cv::VideoCapture cap(1);

	// tries to open 1 of the 2 camera's
	if (!cap.isOpened())
	{
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			std::cout << "Can't open cam exiting" << std::endl;
			exit(0);
		}
	}

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	cv::Mat frame, nonFlipped, hsvFrame, rgbFrame;
	cv::Mat redChannel[3], saturationChannel[3], redValue, satValue, sword;

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
			std::cout << "Can't open cam exiting" << std::endl;
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
		//imshow("Sword", sword);
		//imshow("SamuraiSlicer", frame);

		int fx, fy;
		int lx, ly;
		findFirstPixel(sword, &fx, &fy);
		findLastPixel(sword, &lx, &ly);

		DrawSwordPlaine(fx, fy, lx, ly);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (cv::waitKey(1) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
	return 0;

}

void findFirstPixel(cv::Mat m, int * fX, int * fY)
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
				//std::cout << " First: y = " << y << " X = " << x << "Value = " << std::endl;
				found = true;
				*fY = y;
				*fX = x;
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}

void findLastPixel(cv::Mat m, int * lX, int * lY)
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
				//std::cout << "Last: y = " << y << " X = " << x << "Value = " << std::endl;
				found = true;
				*lY = y;
				*lX = x;
			}
			if (found)
				break;
		}
		if (found)
			break;
	}
}
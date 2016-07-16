#include "opencv2\core\core.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include <mmsystem.h>

using namespace cv;
using namespace std;

#pragma comment(lib, "libs/opencv_core245")
#pragma comment(lib, "libs/opencv_highgui245")
#pragma comment(lib, "libs/opencv_imgproc245")

#include <Windows.h>

Mat image;
Mat original;
int rollCall = 0;
Point Old = Point(0, 0);
Point New = Point(0, 0);

string names[] = {
	"\\ka.jpg",
	"\\kha.jpg",
	"\\ga.jpg",
};

string compareList[] = {
	"\\kaC.jpg",
	"\\khaC.jpg",
	"\\gaC.jpg"
};

string answer[] = {
	"\\Pen.jpg",
	"\\Rabbit.jpg",
	"\\Vase.jpg"
};

int shift = 0;
#define ImageCount 3
#define THREESTAR 0.35
#define TWOSTAR 0.50
#define ONESTART 0.65
bool DRAW = FALSE;
const string windowName = "DigiSlate";

Mat temp(Size(1000, 700), CV_8UC3, Scalar(1, 114, 255));
Rect letterRoi = Rect(200, 0, 600, 600);
//Mat img = imread("\\ka.jpg");

//Mat ball = imread("ball.png");

Mat white(Size(100, 100), CV_8UC3, Scalar(255, 255, 255));
Rect chocRoi1 = Rect(400, 0, 200, 91);
Rect chocRoi2 = Rect(200, 0, 200, 91);
Rect chocRoi3 = Rect(600, 0, 200, 91);
Mat choc = imread("\\choc.png");
void evaluate();

static void onMouse(int event, int x, int y, int f, void*){
	Mat next = imread("\\Next2.jpg");
	Mat previous = imread("\\Pre2.jpg");

	Rect nextRoi = Rect(800, 0, 200, 600);
	Rect previousRoi = Rect(0, 0, 200, 600);
	next.copyTo(temp(nextRoi));
	previous.copyTo(temp(previousRoi));
	New.x = x;
	New.y = y;
	//cout << endl << "Point : " << New;
	//-----------NAVIGATE-----------
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		DRAW = TRUE;
		cout << endl << "START DRAWING";
		cout << endl << "Left Button";
		if (x > 0.8*temp.cols)
		{
			cout << endl << "NEXT PAGE";
			//----EVALUATE NOW---------//

			evaluate();

			rollCall++;
			rollCall %= ImageCount;

			Mat alpha = imread(names[rollCall]);
			alpha.copyTo(temp(letterRoi));
			cout << endl << names[rollCall];
			image = imread(names[rollCall]);
			//imshow("INPUT", image);
			DRAW = FALSE;
			cout << endl << "STOP DRAWING";
		}
		if (x < 0.2*temp.cols)
		{
			cout << endl << "PREVIOUS PAGE";
			//----EVALUATE NOW---------//
			evaluate();

			(rollCall--);
			if (rollCall < 0)
				rollCall = 0;
			cout << endl << names[rollCall];
			image = imread(names[rollCall]);
			Mat alpha = imread(names[rollCall]);
			alpha.copyTo(temp(letterRoi));
			//imshow("INPUT", image);
			DRAW = FALSE;
			cout << endl << "STOP DRAWING";
		}
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		cout << endl << "STOP DRAWING";
		DRAW = FALSE;
	}
	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (DRAW)
		{
			if (x<0.8*temp.cols &&x>0.2*temp.cols)
				line(temp, New, Old, Scalar(0, 0, 0), 24, NULL, NULL);
		}
		Old = New;
		//Rect eraserRoi = Rect((shift * 100) % 1000, 600, 100, 100);
		//Rect ballRoi = Rect((shift++ * 100) % 1000, 600, 100, 100);
		//ball.copyTo(temp(ballRoi));
		//white.copyTo(temp(eraserRoi));
		imshow(windowName, temp);
	}
}

int main() {
	Mat logo = imread("\Logo.jpg");
	namedWindow(windowName);
	imshow(windowName, logo);
	waitKey(1000);
	//img.copyTo(temp(letterRoi));
	//setMouseCallback(windowName, onMouse, 0);
	//waitKey(0000);
	return 0;
}

void evaluate()
{
	Mat pooh = imread("\\pooh.jpg");
	Mat inputImage = temp(letterRoi);
	Mat compareImage = imread(compareList[rollCall]);
	Mat diff;

	//namedWindow("TEST");
	//namedWindow("STIMAGE");
	//namedWindow("DIFF");

	inRange(compareImage, Scalar(0, 0, 0), Scalar(02, 02, 02), compareImage);
	inRange(inputImage, Scalar(0, 0, 0), Scalar(02, 02, 02), inputImage);
	absdiff(compareImage, inputImage, diff);

	double total = sum(compareImage)[0] / (255 * 3);
	double error = sum(diff)[0] / (255 * 3);
	double per = (100.0*error) / total;

	//cout << endl << "TOTAL WHITE : " << total;
	//cout << endl << "Diff : " << sum(diff)[0] / (255 * 3);
	cout << endl << "Percentage : " << per << "error attempting" << names[rollCall];

	if (per < 35)
	{
		cout << endl << "***";
		choc.copyTo(temp(chocRoi1));
		choc.copyTo(temp(chocRoi2));
		choc.copyTo(temp(chocRoi3));
		imshow(windowName, temp);
		waitKey(2000);
	}
	else if (per>35 && per< 50)
	{
		cout << endl << "**";
		choc.copyTo(temp(chocRoi1));
		choc.copyTo(temp(chocRoi2));
		imshow(windowName, temp);
		waitKey(2000);
	}
	else if (per>50 && per < 70)
	{
		cout << endl << "*";
		choc.copyTo(temp(chocRoi1));
		imshow(windowName, temp);
		waitKey(2000);
	}
	else
	{
		cout << endl << ":(";
		pooh.copyTo(temp(Rect(400, 0, 150, 150)));
		imshow(windowName, temp);
		waitKey(3000);
	}
	Mat answerImage = imread(answer[rollCall]);
	imshow(windowName, answerImage);
	waitKey(10);

	DRAW = FALSE;
}
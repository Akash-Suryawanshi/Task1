#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void readVideo(string x) {
	VideoCapture cap(x);

	if (!cap.isOpened()) return;

	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty()) {
			break;
		}
		imshow("Image", frame);
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		}

	}
	cap.release();
	destroyAllWindows();
	return;
}
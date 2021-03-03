#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "transform_crop.h"

using namespace std;
using namespace cv;


int main() {
	transform_and_crop("traffic.jpg");
	return 0;
}
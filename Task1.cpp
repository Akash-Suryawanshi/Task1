#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main() {
	//Mat mypic = transform_and_crop("traffic.jpg");
	readVideo("queue_density_30s.mp4");
	//transform_and_crop("traffic.jpg");
	return 0;
}
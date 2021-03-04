#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"


#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace cv;


vector<Point2f>src_pts;
vector<Point2f>dest_pts;


void init_srcpts() {
	src_pts.push_back(Point2f(942, 230));
	src_pts.push_back(Point2f(289, 956));
	src_pts.push_back(Point2f(1543, 982));
	src_pts.push_back(Point2f(1276, 228));
}


void init() {
	dest_pts.push_back(Point2f(472, 52));
	dest_pts.push_back(Point2f(472, 830));
	dest_pts.push_back(Point2f(800, 830));
	dest_pts.push_back(Point2f(800, 52));
}
 
Mat transform_and_crop(Mat img) {
	init_srcpts();
    init();
	int height = img.rows;
	int width = img.cols;
	Size size(height, width); 
	Mat img_dest = Mat::zeros(size, CV_8UC3); 
	Mat homo = findHomography(src_pts,dest_pts);
	warpPerspective(img, img_dest, homo, size);
	Mat cropedImage = img_dest(Rect(472, 52, 328, 778));
	return cropedImage;
}

void transform_and_crop(string x) { 
	init_srcpts();
	init();
	Mat img = imread(x);
	int height = img.rows;
	int width = img.cols;
	Size size(height, width);
	Mat img_dest = Mat::zeros(size, CV_8UC3);
	Mat homo = findHomography(src_pts, dest_pts);
	warpPerspective(img, img_dest, homo, size);
	Mat cropedImage = img_dest(Rect(472, 52, 328, 778));
	imshow("Cropped Image", cropedImage);
	waitKey(0);
	return;
}








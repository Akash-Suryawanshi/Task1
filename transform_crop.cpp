#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


vector<Point2f>src_pts;
vector<Point2f>dest_pts;


void init_srcpts() {
	src_pts.push_back(Point2f(843, 190));
	src_pts.push_back(Point2f(89, 811));
	src_pts.push_back(Point2f(1516, 944));
	src_pts.push_back(Point2f(1271, 203));
}


void init() {
	dest_pts.push_back(Point2f(472, 52));
	dest_pts.push_back(Point2f(472, 830));
	dest_pts.push_back(Point2f(800, 830));
	dest_pts.push_back(Point2f(800, 52));
}
 
Mat transform_and_crop(string myimage) {
	init_srcpts();
    init();
    Mat img = imread(myimage, IMREAD_GRAYSCALE);
	int height = img.rows;
	int width = img.cols;
	Size size(height, width); 
	Mat img_dest = Mat::zeros(size, CV_8UC3); 
	Mat homo = findHomography(src_pts,dest_pts);
	warpPerspective(img, img_dest, homo, size);
	Mat cropedImage = img_dest(Rect(472, 52, 328, 778));
	return cropedImage;
}








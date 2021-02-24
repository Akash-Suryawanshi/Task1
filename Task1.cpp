#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

vector<Point2f>src_pts;
vector<Point2f>dest_pts;

void getPoint(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN && src_pts.size() < 4) {
		Point2f p(x, y);
		src_pts.push_back(p);
	}
}

void init() {
	dest_pts.push_back(Point2f(472, 52));
	dest_pts.push_back(Point2f(472, 830));
	dest_pts.push_back(Point2f(800, 830));
	dest_pts.push_back(Point2f(800, 52));
}

int main() {

	Mat img = imread("traffic.jpg", IMREAD_GRAYSCALE);

	if (img.empty()) return 0;

	namedWindow("Image", 1);	

	setMouseCallback("Image", getPoint, NULL);

	imshow("Image", img); 

	waitKey(0);
	
	int height = img.rows;
	int width = img.cols;
	
	Size size(height, width); 
	Mat img_dest = Mat::zeros(size, CV_8UC3); 

	Mat homo = findHomography(src_pts,dest_pts);

	warpPerspective(img, img_dest, homo, size);
	
	imshow("Perspective Change",img_dest);
	imwrite("Transformed.jpg", img_dest);
	waitKey(0);
	
	Mat cropedImage = img_dest(Rect(472, 52, 328, 778));
	
	imshow("Final", cropedImage);
	imwrite("CroppedImage.jpg", cropedImage);
	waitKey(0);
	
	return 0;
}

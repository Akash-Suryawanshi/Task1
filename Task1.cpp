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

//testing function
void print_selected_points() {
	for (auto p : src_pts) cout << p << ' ';
	cout << endl;
}

int main() {
	Mat img = imread("Pain.JPG");

	if (img.empty()) return 0;

	namedWindow("Image", 1);	// name the window

	setMouseCallback("Image", getPoint, NULL);

	imshow("Image", img); // show the image

	waitKey(0);

	print_selected_points();

}

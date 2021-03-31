#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	pair <vector<double>, vector<double>>densities;
	densities = readVideo("trafficvideo.mp4");
	// densities = method3_final("trafficvideo.mp4",4);
	vector<double>QueueDensity = densities.first;
	vector<double>DynamicDensity = densities.second;
	cout << "framenum" << "," << "queue density" << "," << "dynamic density";
	cout << endl;
	for (int i = 0; i < 5737; i++) {
		cout << i+1 << ",";
		cout << QueueDensity[i] << ",";
		cout << DynamicDensity[i] << "\n";
		cout << endl;
	}
	return 0;
}
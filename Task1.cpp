#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main() {
	pair <vector<double>, vector<double>>densities;
	time_t start,end;
	time(&start);
	densities = do_parallel("tfv.mp4");
	time(&end);
	// densities = method3("trafficvideo.mp4");
	vector<double>QueueDensity = densities.first;
	vector<double>DynamicDensity = densities.second;
	cout << "framenum" << "," << "queue density" << "," << "dynamic density";
	cout << endl;
	for (int i = 0; i < QueueDensity.size(); i++) {
		cout << i+1 << ",";
		cout << QueueDensity[i] << ",";
		cout << DynamicDensity[i] << "\n";
		cout << endl;
	}
	cout << "Time taken by the program : " << double(end - start) << '\n';
	return 0;
}
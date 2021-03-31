#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

int main() {
	pair <vector<double>, vector<double>>densities;
	time_t start,end;
	time(&start);
	densities = readVideo("trafficvideo.mp4");
	time(&end);
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
	cout << "Time Taken by readVideo : " << setprecision(10) << double(end - start) << endl;
	return 0;
}
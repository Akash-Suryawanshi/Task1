#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main() {
	pair <vector<double>, vector<double>>densities;
	densities = readVideo("tfv.mp4");
	vector<double>QueueDensity = densities.first;
	vector<double>DynamicDensity = densities.second;
	cout << "framenum" << '\t' << '\t' << "queue density" << '\t' << '\t' << "dynamic density";
	cout << endl;
	for (int i = 0; i < QueueDensity.size(); i++) {
		cout << i+1 << '\t' << '\t';
		cout << QueueDensity[i] << '\t' << '\t';
		cout << DynamicDensity[i] << '\t' << '\t';
		cout << endl;
	}
	return 0;
}
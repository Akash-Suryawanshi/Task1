#pragma once
#include "header.h"
#include "iostream"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

Mat transform_and_crop(Mat img);
void init();
void init_srcpts();
pair<vector<double>,vector<double>> readVideo(string x);
void transform_and_crop(string x);
pair<vector<double>,vector<double>> method3(string x);
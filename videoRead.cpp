#include <iostream>
#include "header.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
int k = 0.5;
// Methods
bool GBlur_and_Threshold = true;

double areaQueue(Mat img) {
	double ans = 0;
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++) {
			if(img.at<cv::Vec3b>(i,j)[0] == 255.0) {
				ans++;
			}
		}
	} 
	return ans/(img.rows * img.cols);
}
double areaDynamic(Mat img) {
	double ans = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img.at<cv::Vec3b>(i, j)[0] == 255.0) {
				ans++;
			}
		}
	}
	return ans / (img.rows * img.cols);
}
pair<vector<double>,vector<double>> readVideo(string x) {
	vector<double> contourAreasQueue;
	vector<double> contourAreasDynamic;	
	Ptr<BackgroundSubtractor> pKNN; // MOG2 background subtractor
	double varThreshold = 50;
	int niters = 2;


	VideoCapture cap(x);
	// cap.set(3, 320);
	// cap.set(4, 244);
	Mat Dframe; //fg mask generated by MOG2 method
	pKNN = bgsegm::createBackgroundSubtractorGSOC(); // background subtractor objects
	// virtual void bgsegm::createBackgroundSubtractorMOG.setVarThreshold(varThreshold);
	Mat emt = imread("frame.jpg");
	Mat emt0;
	Mat emptyBG0 = transform_and_crop(emt);
	resize(emptyBG0, emptyBG0, Size(120, 240));
	cout << emptyBG0.size() << endl;
	
	Mat emptyBG;
	cvtColor(emptyBG0, emptyBG, COLOR_BGR2GRAY);
	
	
	
	
	bool first = true;
	while (1) {
		Mat frame; 
		cap >> frame;
		
		int X = 0; // We skip next X frames
		if(!first){
			bool status;
			for(int i=0; i<X; i++){
				status = cap.read(frame);
			}
		}

		if (frame.empty()) {
			break;
		}
		
		
		Mat croppedFrame = transform_and_crop(frame);
		imshow("frame.jpg", croppedFrame);
		resize(croppedFrame, croppedFrame, Size(120, 240));

		pKNN->apply(croppedFrame, Dframe);
		imshow("FG MASK", Dframe);


		Mat croppedFrame0 = croppedFrame;
		cvtColor(croppedFrame, croppedFrame0, COLOR_BGR2GRAY);
		GaussianBlur(croppedFrame0, croppedFrame0, Size(3, 3), 0);

		Mat diffImage0;
		absdiff(croppedFrame0, emptyBG,diffImage0);
		imshow("diffImage0", diffImage0);
		
		threshold(diffImage0, diffImage0, 50, 255, THRESH_BINARY);
    	dilate(diffImage0, diffImage0, Mat(), Point(-1, -1), niters*2);
		imshow("diffImage0_after", diffImage0);
		
		//contourAreasQueue.push_back(areaQueue(Qframe));
		//contourAreasDynamic.push_back(areaDynamic(Dframe));
		double QueueArea = areaQueue(diffImage0);
		double DynamicArea = areaDynamic(Dframe);
		QueueArea = max(QueueArea, DynamicArea);

		for(int i=0; i<=X; i++){
				contourAreasQueue.push_back(QueueArea);
				contourAreasDynamic.push_back(DynamicArea);
		}
		
		first = false;
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		} 
	}
	cap.release();
	destroyAllWindows();
	ofstream MyFile("BaseLineQueue.txt");
	for (int i = 0; i < 5737; i++){
		MyFile <<  contourAreasQueue[i] << endl;
	}
	MyFile.close();
	ofstream MyFile2("BaseLineDynamic.txt");
	for (int i = 0; i < 5737; i++){
		MyFile2 <<  contourAreasDynamic[i] << endl;
	}
	MyFile2.close();
	return make_pair(contourAreasQueue,contourAreasDynamic);
}
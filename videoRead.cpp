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

double areaQueue(Mat img) {
	double ans = 0;
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++) {
			if(img.at<cv::Vec3b>(i,j)[0] == 255.0) {
				ans++;
			}
		}
	} 
	return ans/(img.rows * img.cols)*1.25;
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
	VideoCapture cap(x);
	// cap.set(3, 320);
	// cap.set(4, 244);
	Mat Dframe; //fg mask generated by MOG2 method
	Ptr<BackgroundSubtractor> pKNN; // MOG2 background subtractor
	double varThreshold = 50;
		
	pKNN = bgsegm::createBackgroundSubtractorGSOC(); // background subtractor objects
	// virtual void bgsegm::createBackgroundSubtractorMOG.setVarThreshold(varThreshold);
	Mat emt = imread("emptyRoad.jpg");
	// resize(emt, emt, Size(320, 244), INTER_CUBIC);
	Mat emptyBG0 = transform_and_crop(emt);
	cout << emptyBG0.size() << endl;
	
	Mat emptyBG;
	cvtColor(emptyBG0, emptyBG, COLOR_BGR2GRAY);
	
	vector<double> contourAreasQueue;
	vector<double> contourAreasDynamic;	
	
	ofstream MyFile("hull_Q.txt");

	while (1) {
		Mat frame; 
		cap >> frame;

		int X = 5;
		bool status;
		for(int i=0; i<X; i++){
			status = cap.read(frame);
		}

		if (frame.empty()) {
			break;
		}
		
		
		// resize(frame, frame, Size(320, 244), 0, 0, INTER_CUBIC);

		Mat croppedFrame = transform_and_crop(frame);
		pKNN->apply(croppedFrame, Dframe);
		imshow("FG MASK", Dframe);
		
		Mat diffImage;
		absdiff(emptyBG0,croppedFrame,diffImage);
		Mat Qframe = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
		float threshold = 77.0f;
    	float dist;
		
		for(int j=0; j<diffImage.rows; ++j){
        	for(int i=0; i<diffImage.cols; ++i){
				cv::Vec3b pix = diffImage.at<cv::Vec3b>(j,i);
				dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
				dist = sqrt(dist);
				if(dist>threshold){            
					Qframe.at<unsigned char>(j,i) = 255;
				}
			}
		}
		imshow("Difference Image", Qframe);
		
		contourAreasQueue.push_back(areaQueue(Qframe));
		contourAreasDynamic.push_back(areaDynamic(Dframe));
		
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		} 
	}
	cap.release();
	destroyAllWindows();
	
	for (int i = 0; i < contourAreasQueue.size(); i++){
		MyFile <<  contourAreasQueue[i] << endl;
	}

	MyFile.close();

	return make_pair(contourAreasQueue,contourAreasDynamic);
}
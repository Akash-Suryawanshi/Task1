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

double area(Mat img) {
	double ans = 0;
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++) {
			if(img.at<cv::Vec3b>(i,j)[0] == 255.0) {
				ans++;
			}
		}
	}
	return ans/(img.rows*img.cols);
}

pair<vector<double>,vector<double>> readVideo(string x) {
	VideoCapture cap(x);
	Mat fgMaskMOG2; //fg mask generated by MOG2 method
	Ptr<BackgroundSubtractor> pKNN; // MOG2 background subtractor
	double varThreshold = 50;
		
	pKNN = bgsegm::createBackgroundSubtractorGSOC(); // background subtractor objects
	// virtual void bgsegm::createBackgroundSubtractorMOG.setVarThreshold(varThreshold);
	Mat emptyBG0 = transform_and_crop(imread("emptyRoad.jpg"));
	Mat emptyBG;
	cvtColor(emptyBG0, emptyBG, COLOR_BGR2GRAY);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector <vector<Point>> contoursD; // contours for difference img
	vector<Vec4i> hierarchyD;
	vector<double> contourAreasQueue;
	vector<double> contourAreasDynamic;	// countour areas per frame
	vector<double> time;
	double t = 0;
	ofstream MyFile("Queue_area_GSOC.txt");
	while (1) {
		Mat frame; // current frame
		cap >> frame;

		if (frame.empty()) {
			break;
		}
		Mat croppedFrame = transform_and_crop(frame);
		pKNN->apply(croppedFrame, fgMaskMOG2);
		imshow("FG MASK", fgMaskMOG2);
		// emptyBG0.cols, emptyBG0.rows
		Mat diffImage;
		absdiff(emptyBG0,croppedFrame,diffImage);
		Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
		float threshold = 77.0f;
    	float dist;
		for(int j=0; j<diffImage.rows; ++j){
        	for(int i=0; i<diffImage.cols; ++i){
            cv::Vec3b pix = diffImage.at<cv::Vec3b>(j,i);
            dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
            dist = sqrt(dist);
            if(dist>threshold){            
                foregroundMask.at<unsigned char>(j,i) = 255;
            }
        }
	}
		imshow("Difference Image", foregroundMask);
		contourAreasQueue.push_back(area(foregroundMask));
		contourAreasDynamic.push_back(area(fgMaskMOG2));
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
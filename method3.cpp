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
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
using namespace std;
using namespace cv;

int NUM_OF_THREADS = 2;
//Globals : 
vector<double> contourAreasQueue;
vector<double> contourAreasDynamic;	
Ptr<BackgroundSubtractor> pKNN; //background subtractor
double varThreshold = 50;
int niters = 4;
Mat Dframe; // fg mask generated by background subtractor
Mat emptyBG; // empty Background
Mat emptyBG0;
string file; // name of file
Mat frame; // frame to be processed
Mat croppedFrame;
Mat croppedFrame0;
Mat diffImage;
Mat diffImage0;
Mat Qframe;
int rc;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
// helper functions
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
//
void initialize(){
	pKNN = bgsegm::createBackgroundSubtractorGSOC(); // background subtractor objects
	Mat emt = imread("frame.jpg");
	Mat emt0;
	emptyBG0 = transform_and_crop(emt);
	resize(emptyBG0, emptyBG0, Size(360, 240)); // ig this is Method 2
    cvtColor(emptyBG0, emptyBG, COLOR_BGR2GRAY);
}

void part1(){
	croppedFrame = transform_and_crop(frame);
	imshow("frame.jpg", croppedFrame);
	resize(croppedFrame, croppedFrame, Size(360, 240));
}

void part2(){
	pKNN->apply(croppedFrame, Dframe);
	imshow("FG MASK", Dframe);
}

void part3(){
	croppedFrame0 = croppedFrame;
	cvtColor(croppedFrame, croppedFrame0, COLOR_BGR2GRAY);
	GaussianBlur(croppedFrame0, croppedFrame0, Size(3, 3), 0);

}

void part4(){
	absdiff(croppedFrame0, emptyBG,diffImage0);
	imshow("diffImage0", diffImage0);	
}

void part5(){
	threshold(diffImage0, diffImage0, 50, 255, THRESH_BINARY);
    dilate(diffImage0, diffImage0, Mat(), Point(-1, -1), niters*2);
	imshow("diffImage0_after", diffImage0);
}

void part6(){
	absdiff(croppedFrame, emptyBG0,diffImage);
 	Qframe = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
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
}

void part7(){
	Qframe = max(Qframe,Dframe);
	contourAreasQueue.push_back(areaQueue(Qframe));
	contourAreasDynamic.push_back(areaDynamic(Dframe));
}

void Threads_1 (){
	VideoCapture cap(file);
	while(1){
		cap >> frame;
		if(frame.empty()) break;
		part1();
		part2();
		part3();
		part4();
		part5();
		part6();
		part7();
		char c = (char)waitKey(25);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}
void *execute2(void *tmp){
	cout << "HELP\n";
	pthread_exit(NULL);
}
void *execute21(void *tmp){
	part1();
	part2();
	part3();
	part4();
	part5();
	part6();
	part7();
	pthread_exit(NULL);
}
void Threads_2(){
	VideoCapture cap(file);
	pthread_t threads[NUM_OF_THREADS]; //2
	//rc = pthread_create(&threads[i],NULL,execute2);
	//rc = pthread_create(&threads[i],NULL,execute21);
	while(1){
		cap >> frame;
		if(frame.empty()) break;
		int i;
		rc = pthread_create(&threads[0],NULL,execute2,(void *) i);
		rc = pthread_create(&threads[1],NULL,execute21,(void *) i);
		pthread_join(threads[0],NULL);
		pthread_join(threads[1],NULL);
		char c = (char)waitKey(25);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}
pair<vector<double>,vector<double>> method3(string x){
	file = x;
	initialize();
	Threads_2();
	ofstream MyFile("hull_Q.txt");
	for (int i = 0; i < contourAreasQueue.size(); i++){
		MyFile <<  contourAreasQueue[i] << endl;
	}
	MyFile.close();
	return make_pair(contourAreasQueue,contourAreasDynamic);
}
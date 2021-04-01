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
#include <pthread.h>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int k = 0.5;
int X;
int NUM_THREADS = 4;
vector<pair<int,double>> contourAreasQueue;
vector<pair<int,double>> contourAreasDynamic;
bool first;
// Methods
bool GBlur_and_Threshold = true;

Ptr<BackgroundSubtractor> pKNN; // MOG2 background subtractor
Mat emptyBG0;
vector<Mat>frames;

bool compare(pair<long,double>const &i, pair<long,double> const &j){
	return i.first >= j.first;
}

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

void *method4(void *tid) {
	Mat frame = frames[(long) tid];

	double varThreshold = 50;
	int niters = 4;

	// cap.set(3, 320);
	// cap.set(4, 244);
	Mat Dframe; //fg mask generated by MOG2 method
	// virtual void bgsegm::createBackgroundSubtractorMOG.setVarThreshold(varThreshold);
	Mat emt0;
	
	resize(emptyBG0, emptyBG0, Size(360, 240));
	cout << emptyBG0.size() << endl;
	
	
	Mat emptyBG;
	cvtColor(emptyBG0, emptyBG, COLOR_BGR2GRAY);

	Mat croppedFrame = frame;
	cout << "YES\n";
	imshow("frame.jpg", croppedFrame);
	resize(croppedFrame, croppedFrame, Size(360, 240));

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

		
		
	Mat diffImage;
	absdiff(croppedFrame, emptyBG0,diffImage);
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
		
	//contourAreasQueue.push_back(areaQueue(Qframe));
	//contourAreasDynamic.push_back(areaDynamic(Dframe));
	double QueueArea = areaQueue(Qframe);
	double DynamicArea = areaDynamic(Dframe);
	QueueArea = max(QueueArea,DynamicArea);

	pthread_mutex_lock(&mutex1);
	contourAreasQueue.push_back(make_pair((long) tid,QueueArea));
	contourAreasDynamic.push_back(make_pair((long) tid,DynamicArea));
	cout << QueueArea << ',' << DynamicArea << endl;		
	pthread_mutex_unlock(&mutex1);
	
	pthread_exit(NULL);
}

pair<vector<double>,vector<double>> do_parallel(string x){
    vector<double>QD,DD;
    int frame_count = 0;
	int t;
	
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
   	void *Status;

    VideoCapture cap(x);
    ofstream MyFile("hull_Q.txt");
    first = true;
    pKNN = bgsegm::createBackgroundSubtractorGSOC(); // background subtractor objects
    Mat emt = imread("frame.jpg");
	emptyBG0 = transform_and_crop(emt);
	
	while (1) {
		Mat frame; 
		cap >> frame;
		
		frames.resize(NUM_THREADS);
		X = NUM_THREADS; // We process next X frames in X different threads
		
		bool status;
		for(int i=0; i<X; i++){
			frames[i] = frame;
			status = cap.read(frame);
		}


		if (frame.empty()) {
			break;
		}
		
		for(int i=0; i<X; i++){
			frames[i] = transform_and_crop(frames[i]);
		}

		// call threading function
		pthread_attr_init(&attr);
   		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for(int i=0; i<X; i++){
			cout << "Created Thread : " << i << '\n';
			int rc = pthread_create(&threads[i],NULL,method4,(void *)i);
			if(rc){
				cout << "Could not create thread\n";
				exit(1);
			}
   		}
		//join threads
		pthread_attr_destroy(&attr);
		for(int i = 0; i < NUM_THREADS; i++ ) {
      		int rc = pthread_join(threads[i], &Status);
      		if (rc) {
         		cout << "Error:unable to join," << rc << endl;
  		        exit(-1);
      		}   
   		}
		//
		frames.clear();
		//first sort the vectors
		/*std::sort(contourAreasQueue.begin(),contourAreasQueue.end(),compare);
		std::sort(contourAreasDynamic.begin(),contourAreasDynamic.end(),compare);*/
		for(int i=0; i<X; i++){
			double Q = contourAreasQueue[i].second;
			double D = contourAreasDynamic[i].second;
			QD.push_back(Q);
			DD.push_back(D);
		}
		contourAreasDynamic.clear();
		contourAreasQueue.clear(); 

		first = false;
		char c = (char)waitKey(25);
		if (c == 27) {
			break;
		}
	}
	cap.release();
	destroyAllWindows();
	    
	for (int i = 0; i < QD.size(); i++){
		MyFile <<  QD[i] << endl;
	}

	MyFile.close();
	return make_pair(QD,DD);
}


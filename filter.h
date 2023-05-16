#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "filter.h"
#include <iostream>
#include<cmath>

using namespace cv;
using namespace std;

int greyscale(Mat frame, Mat& edittedFrame);
int gaussBlur(Mat frame, Mat& edittedFrame);
int sobelx(Mat frame, Mat3s& edittedFrame);
int sobely(Mat frame, Mat3s& edittedFrame);
int magnitude(Mat sobelx, Mat sobely, Mat& edittedFrame);
int blurQuantize(Mat frame, Mat& edittedFrame, int levels);
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold);
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "filter.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    // initializing frame to apply filter on
    cv::Mat filteredFrame;
    // variable to hold current filter status
    std::string status = "ORIGINAL";
    // temp frame to hold frames to get absolutes
    cv::Mat3s temp;
    // other frames for holding mats
    cv::Mat sx, sy;

    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        if (status == "ORIGINAL") {
            filteredFrame = frame;
        }
        if (status == "GRAY") {
            frame.copyTo(filteredFrame);
            greyscale(frame, filteredFrame);
        }
        if (status == "BLUR") {
            frame.copyTo(filteredFrame);
            gaussBlur(frame, filteredFrame);
        }
        if (status == "SOBELX") {
            frame.copyTo(temp);
            sobelx(frame, temp);
            convertScaleAbs(temp, filteredFrame);
        }
        if (status == "SOBELY") {
            frame.copyTo(temp);
            sobely(frame, temp);
            convertScaleAbs(temp, filteredFrame);
        }
        if (status == "MAGNITUDE") {
            frame.copyTo(temp);
            sobelx(frame, temp);
            convertScaleAbs(temp, sx);
            frame.copyTo(temp);
            sobely(frame, temp);
            convertScaleAbs(temp, sy);
            magnitude(sx, sy, filteredFrame);
        }
        if (status == "BLURQUANTIZE") {
            frame.copyTo(filteredFrame);
            blurQuantize(frame, filteredFrame, 15);
        }
        if (status == "CARTOONIZE") {
            frame.copyTo(filteredFrame);
            cartoon(frame, filteredFrame, 15, 19);
        }

        cv::imshow("Video", filteredFrame);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
        else if (key == 'o') {
            status = "ORIGINAL";
        }
        else if (key == 'g') {
            status = "GRAY";
        }
        else if (key == 'b') {
            status = "BLUR";
        }
        else if (key == 'x') {
            status = "SOBELX";
        }
        else if (key == 'y') {
            status = "SOBELY";
        }
        else if (key == 'm') {
            status = "MAGNITUDE";
        }
        else if (key == 'z') {
            status = "BLURQUANTIZE";
        }
        else if (key == 'c') {
            status = "CARTOONIZE";
        }
    }

    delete capdev;
    return(0);
}
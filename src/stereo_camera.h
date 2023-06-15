//
// Created by Kevin Williams on 6/13/23.
// https://www.cppstories.com/2019/12/threading-loopers-cpp17/
//

#pragma once

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"
#include "constants.h"
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <vector>
#include "imagemapper.h"
#include "nanoutils.h"
using namespace std;

class StereoCamera {

public:

    uint8_t device1 = 0;
    uint8_t device2 = 1;

    cv::Mat value1;
    cv::Mat value2;

    cv::Mat mapped_left;
    cv::Mat mapped_right;
    cv::Mat mapped_3d;

    cv::VideoCapture * cap1;
    cv::VideoCapture * cap2;

    vector<uchar> jpeg;

    ImageMapper * mapper;

    bool running() const 
    {
        return mRunning.load();
    }

    StereoCamera() {

        mapper = new ImageMapper();

        this->cap1 = new cv::VideoCapture(_gst_pipeline(device1, nano::kDefaultCaptureSettings));
        this->cap2 = new cv::VideoCapture(_gst_pipeline(device2, nano::kDefaultCaptureSettings));

        printf("\nOPENING CAMERAS...");

        if(!cap1->isOpened() || !cap2->isOpened()){
            printf("...FAIL");
            exit(0);
        }
    
        printf("...SUCCESS.\n\n");

        printf("READING CAMERAS...");
        bool r1 = cap1->read(value1);
        bool r2 = cap2->read(value2);

        if(!r1 || !r2) {
            printf("...FAIL");
        } else {
            printf("...SUCCESS.\n\n");
        }
    }

    virtual ~StereoCamera() {
        abortAndJoin();
        release();
    }

    void release(){
        cap1->release();
        cap2->release();
    }

    bool capture(cv::Mat & m1, cv::Mat & m2){

        bool r1 = cap1->read(m1);
        bool r2 = cap2->read(m2);

        if(!(r1 && r2)) {
            printf("couldn't read image from camera");
            return false;
        } else {
            printf("got values");
        }

        return true;
    }

    void save_image(){

        cv::Mat im1;
        cv::Mat im2;

        if(cap1->read(im1)){
            cv::imwrite("im1.png", im1);
        } else {
            printf("couldn't read image l");
        }

        if(cap1->read(im2)){
            cv::imwrite("im2.png", im2);
        } else {
            printf("couldn't read image r");
        }
        

        cap2->read(im2);

        
        cv::imwrite("im1.png", im1);
        cv::imwrite("im2.png", im2);

    }
    
    void loop() {
        mRunning.store(true);

        while(true) {
            try {
                usleep(50);
                _read();
            } catch (...) {
                
            }
        }

        mRunning.store(false);
    }

    void run(){
        try {
            mThread = std::thread(&StereoCamera::loop, this);
        } catch(...) {
            printf("could not run thread");
        }
    }

    void stop(){
        abortAndJoin();
    }

    string _gst_pipeline(
            int device_id,
            const capture_settings_t &settings
    ) {
        return "nvarguscamerasrc sensor-id=" + std::to_string(device_id) + " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(settings.capture_width) + ", height=(int)" +
            std::to_string(settings.capture_height) + ", framerate=(fraction)" + std::to_string(settings.framerate) +
            "/1 ! nvvidconv flip-method=" + std::to_string(settings.flip_method) + " ! video/x-raw, width=(int)" + std::to_string(settings.display_width) + ", height=(int)" +
            std::to_string(settings.display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    }


    bool _read() {
        
        cv::Mat img1;
        cv::Mat img2;

        if(cap1->isOpened() && cap2->isOpened()) {
            bool r1 = cap1->read(value1);
            bool r2 = cap2->read(value2);
            if(r1 && r2) {

                mapper->remap(value1, value2, mapped_left, mapped_right);
                mapper->map_3d(mapped_left, mapped_right, mapped_3d);
                nano::bgr8_to_jpeg(mapped_3d, jpeg);
                return true;
            }
        }
        return false;
    
    }

    char * get_image() {
        return (char*)jpeg.data();
    }

    void abortAndJoin()
    {
        mAbortRequested.store(true);
        if(mThread.joinable())
        {
            mThread.join();
        }
    }
    
private:

    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested;

};



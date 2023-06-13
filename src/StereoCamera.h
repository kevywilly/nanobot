//
// Created by Kevin Williams on 6/13/23.
//

#ifndef NANOBOT_STEREOCAMERA_H
#define NANOBOT_STEREOCAMERA_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "constants.h"
using namespace std;



class StereoCamera {

public:

    StereoCamera();

    virtual ~StereoCamera();


private:
    cv::VideoCapture * _cap_l;
    cv::VideoCapture * _cap_r;
    uint8_t _left_id = 0;
    uint8_t _right_id = 1;

    static string _gst_pipeline(
        int device_id,
        int capture_width,
        int capture_height,
        int display_width,
        int display_height,
        int framerate,
        int flip_method
    ) {
        return "nvarguscamerasrc sensor-id=" + std::to_string(device_id) + " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
            std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
            "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
            std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    }

    static void _start_camera(uint8_t id, cv::VideoCapture * cap) {
        printf("starting camera");
        cap = new cv::VideoCapture(
                _gst_pipeline(id,CAPTURE_WIDTH, CAPTURE_HEIGHT, DISPLAY_WIDTH, DISPLAY_HEIGHT, FRAMERATE, FLIP_METHOD)
        );
        if(!cap->isOpened()) {
            printf("Error opening camera %d",id);
            exit(-1);
        }
    }

    void release() {
        if(_cap_l != nullptr && _cap_l->isOpened()) {
            _cap_l->release();
        }

        if(_cap_r != nullptr && _cap_r->isOpened()) {
            _cap_r->release();
        }
    }
};


#endif //NANOBOT_STEREOCAMERA_H

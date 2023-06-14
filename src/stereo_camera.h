//
// Created by Kevin Williams on 6/13/23.
//

#ifndef NANOBOT_STEREOCAMERA_H
#define NANOBOT_STEREOCAMERA_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "types.h"
#include "constants.h"

using namespace std;


class StereoCamera {

public:

    StereoCamera();

    virtual ~StereoCamera();

    void release();

private:
    cv::VideoCapture * _cap_l;
    cv::VideoCapture * _cap_r;
    uint8_t _left_id = 0;
    uint8_t _right_id = 1;

    static string _gst_pipeline(
            int device_id,
            const capture_settings_t &settings
    ) {
        return "nvarguscamerasrc sensor-id=" + std::to_string(device_id) + " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(settings.capture_width) + ", height=(int)" +
            std::to_string(settings.capture_height) + ", framerate=(fraction)" + std::to_string(settings.framerate) +
            "/1 ! nvvidconv flip-method=" + std::to_string(settings.flip_method) + " ! video/x-raw, width=(int)" + std::to_string(settings.display_width) + ", height=(int)" +
            std::to_string(settings.display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    }

    static void _start_camera(uint8_t id, cv::VideoCapture * cap) {
        printf("starting camera");
        cap = new cv::VideoCapture(
                _gst_pipeline(id,nb::kDefaultCaptureSettings)
        );
        if(!cap->isOpened()) {
            printf("Error opening camera %d",id);
            exit(-1);
        }
    }

};


#endif //NANOBOT_STEREOCAMERA_H

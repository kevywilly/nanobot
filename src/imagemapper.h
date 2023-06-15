#pragma once
/**
 https://docs.opencv.org/3.4/d1/da0/tutorial_remap.html
 https://learnopencv.com/making-a-low-cost-stereo-camera-using-opencv/
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "types.h"
#include <iostream>

using namespace std;

class ImageMapper {
    public:

    cv::Mat left_map_1;
    cv::Mat left_map_2;
    cv::Mat right_map_1;
    cv::Mat right_map_2;
    bool model_loaded = false;

    ImageMapper() {
        file_settings_t settings;
        auto filename = settings.modelFolder + "/" + settings.d3MapFile;
        cout << "loading model file" << filename << endl;
        cv::FileStorage fs(filename, cv::FileStorage::READ);
        if(fs.isOpened()) {
            fs["left_map_1"] >> left_map_1;
            fs["left_map_2"] >> left_map_2;
            fs["right_map_1"] >> right_map_1;
            fs["right_map_2"] >> right_map_2;
            model_loaded = true;
        }
        cout << "...loaded model file";
    }

    void remap(cv::Mat left, cv::Mat right, cv::Mat &out_left, cv::Mat &out_right) {
        // INTER_LANCZOS4
        // WARP_FILL_OUTLIERS
        if(model_loaded) {
            cv::remap(left, out_left, left_map_1, left_map_2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
            cv::remap(right, out_right, right_map_1, right_map_2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0,0,0));
            crop(out_left, 0.90);
            crop(out_right,0.90);
        } else {
            left.copyTo(out_left);
            right.copyTo(out_right);
        }
    }

    void map_3d(cv::Mat left, cv::Mat right, cv::Mat &out) {
        cv::Mat left_split[3], right_split[3];
 
        std::vector<cv::Mat> channels;
        
        cv::split(left, left_split);
        cv::split(right, right_split);
        
        channels.push_back(right_split[0]);
        channels.push_back(right_split[1]);
        channels.push_back(left_split[2]);
        
        cv::merge(channels, out);
    }

    void crop(cv::Mat &img, float pct) {

        int w = img.size().width;
        int h = img.size().height;

        if (pct == 1.0) {
            return;
        }

        auto fact = 1.0 - pct;
        int yfactor = int(h * fact / 2);
        int xfactor = int(w * fact / 2);
        img = img(cv::Range(yfactor,h-yfactor), cv::Range(xfactor,w-xfactor));
        
    }

};
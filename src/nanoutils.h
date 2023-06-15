#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace nano {
    bool bgr8_to_jpeg(cv::Mat value, std::vector<uchar> &buf){
        cv::imencode(".jpg", value, buf);
        return true;
    }

    bool stereo_to_jpeg(cv::Mat left, cv::Mat right, std::vector<uchar> &buf){
        cv::Mat out;
        cv::hconcat(left, right, out);
        cv::imencode(".jpg", out, buf);
        return true;
    }
}
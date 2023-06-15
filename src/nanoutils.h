#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace nano {
    bool bgr8_to_jpeg(cv::Mat value, std::vector<uchar> &buf){
        cv::imencode(".jpg", value, buf);
        return true;
    }
}
//
// Created by Kevin Williams on 6/13/23.
//

#include "stereo_camera.h"
#include "constants.h"

StereoCamera::StereoCamera() {
    _start_camera(_left_id, _cap_l);
    _start_camera(_right_id, _cap_r);
}

StereoCamera::~StereoCamera() {
   release();
}

void StereoCamera::release() {
    if(_cap_l != nullptr && _cap_l->isOpened()) {
        _cap_l->release();
    }

    if(_cap_r != nullptr && _cap_r->isOpened()) {
        _cap_r->release();
    }
}

void StereoCamera::capture() {
  cv::Mat img_left;
  cv::Mat img_right;
  bool r1 = _cap_l->read(img_left);
  bool r2 = _cap_r->read(img_right);

  if(!(r1 && r2)) {
    printf("couldn't read image from camera");
  } else {
    value_left = img_left;
    value_right = img_right;
  }

}

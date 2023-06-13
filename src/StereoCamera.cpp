//
// Created by Kevin Williams on 6/13/23.
//

#include "StereoCamera.h"
#include "constants.h"

StereoCamera::StereoCamera() {
    _start_camera(_left_id, _cap_l);
    _start_camera(_right_id, _cap_r);
}

StereoCamera::~StereoCamera() {
   release();
}


//
// Created by Kevin Williams on 6/13/23.
//

#ifndef NANOBOT_TYPES_H
#define NANOBOT_TYPES_H

#include <string>

typedef struct {
    int capture_width;
    int capture_height;
    int display_width;
    int display_height;
    int framerate;
    int flip_method;
} capture_settings_t;

typedef struct file_settings_t{
    std::string data_folder = "/ml_data";
    std::string modelFolder = "/ml_data/calibration/models";
    std::string d3MapFile = "3dmap.xml";
} file_settings_t;

/*
base_folder: str = "/ml_data/calibration"
    model_folder: str = "/ml_data/calibration/models"
    model_file: str = "/ml_data/calibration/models/camera_model.xml"
    rectification_model_file: str = "/ml_data/calibration/models/rectification_model.xml"
    d3_map_file: str = "/ml_data/calibration/models/3dmap.xml"

    output_folder = f"{base_folder}/images/output"
    right_folder = f"{base_folder}/images/stereo/right"
    left_folder = f"{base_folder}/images/stereo/left"

*/

#endif //NANOBOT_TYPES_H

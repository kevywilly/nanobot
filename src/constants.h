//
// Created by Kevin Williams on 6/13/23.
//



#ifndef NANOBOT_CONSTANTS_H
#define NANOBOT_CONSTANTS_H
#include <stdlib.h>
#include "types.h"


namespace nb {
    const capture_settings_t kDefaultCaptureSettings = {
            .capture_width = 1920,
            .capture_height = 1080,
            .display_width = 640,
            .display_height = 360,
            .framerate = 30,
            .flip_method = 2
    };
}




#endif //NANOBOT_CONSTANTS_H

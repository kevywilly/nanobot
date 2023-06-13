#include <iostream>
#include "src/StereoCamera.h"
#include <unistd.h>
int main() {
    std::cout << "Hello, World!" << std::endl;

    StereoCamera camera;

    while(1) {
        usleep(50);
    }

    camera.release();
    return 0;
}

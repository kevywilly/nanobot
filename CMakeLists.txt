#cmake_minimum_required(VERSION 3.22)
#project(nanobot)

#set(CMAKE_CXX_STANDARD 14)

#add_executable(nanobot main.cpp)

cmake_minimum_required(VERSION 3.10)

# set the project name
project(nanobot)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")


# specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-fPIC -g -pedantic -Wall -Wextra -pthread -std=c++20")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(LOG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOG=1")
endif()

find_package(OpenCV 4.5 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR} src)

#add_subdirectory(CSI_Camera)
#include_directories(CSI_Camera/src)
#include_directories(src)

# Build the calibration tool
add_executable(nanobot main.cpp  src/mongoose.c src/parson.c)
target_link_libraries(nanobot ${OpenCV_LIBRARIES})

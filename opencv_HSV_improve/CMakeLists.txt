cmake_minimum_required(VERSION 3.14)
project(opencv_HSV_improve)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
add_executable(opencv_HSV_improve main.cpp)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
cmake_minimum_required(VERSION 3.0.0)
project( garden_defender VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(garden_defender main.cpp)

target_link_libraries( garden_defender ${OpenCV_LIBS} )

include(CPack)
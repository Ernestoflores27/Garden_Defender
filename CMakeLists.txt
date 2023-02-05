cmake_minimum_required(VERSION 3.0.0)
project( garden_defender VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(PiPCA9685 REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} ${pigpio_INCLUDE_DIR})

add_executable(garden_defender main.cpp)

target_link_libraries(garden_defender ${OpenCV_LIBS} ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY} ${PiPCA9685_LIBRARY})
target_link_libraries(garden_defender -lpigpio)

include(CPack)
cmake_minimum_required(VERSION 3.0.0)
project(garden_defender VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(PiPCA9685 REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} ${pigpio_INCLUDE_DIR})


SET(TARGET_H Objects/Camera.h
Objects/Camera.h
Objects/Detector.h
Objects/Object.h
Objects/Servos.h
Objects/Turret.h
)

SET (TARGET_SRC main.cpp)

add_executable(garden_defender ${TARGET_SRC})

target_link_libraries(garden_defender ${OpenCV_LIBS} ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY} ${PiPCA9685_LIBRARY})
target_link_libraries(garden_defender -lpigpio -pthread)

include(CPack)

enable_testing()

add_executable(TestTurret test/TestTurret.cpp)
add_executable(TestDetector test/TestDetector.cpp)
add_executable(TestServos test/TestServos.cpp)

target_link_libraries(TestTurret ${OpenCV_LIBS} ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY} ${PiPCA9685_LIBRARY})
target_link_libraries(TestTurret -lpigpio -pthread -lgtest -lgtest_main)

target_link_libraries(TestDetector ${OpenCV_LIBS} ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY} ${PiPCA9685_LIBRARY})
target_link_libraries(TestDetector -lpigpio -pthread -lgtest -lgtest_main)


target_link_libraries(TestServos ${OpenCV_LIBS} ${pigpio_LIBRARY} ${pigpiod_if_LIBRARY} ${pigpiod_if2_LIBRARY} ${PiPCA9685_LIBRARY})
target_link_libraries(TestServos -lpigpio -pthread -lgtest -lgtest_main)

include(GoogleTest)
gtest_discover_tests(TestTurret)
gtest_discover_tests(TestDetector)
gtest_discover_tests(TestServos)
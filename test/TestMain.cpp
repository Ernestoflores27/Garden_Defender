#include "gtest/gtest.h"
#include "../Objects/Camera.cpp"
#include "../Objects/Detector.cpp"
#include <opencv4/opencv2/opencv.hpp>

// Test fixture class for the Camera class.
class CameraTest : public ::testing::Test {
 protected:
  // Camera cam;

  void SetUp() override {}

  void TearDown() override {}
};

// Test the Camera class by checking if the returned video capture object is open.
TEST_F(CameraTest, VideoCaptureTest) {
  Camera cam(640, 480);
  cv::VideoCapture real_time = cam.getVideoCapture();
  EXPECT_TRUE(real_time.isOpened());
  real_time.release();
}

// Test the Detector class by checking if it detects any object.
TEST(DetectorTest, DetectionTest) {
  Camera cam1(640, 480);
  cv::VideoCapture real_time = cam1.getVideoCapture();
  // cv::VideoCapture real_time("../test/test.mp4");

  Turret turret(23, 24, 25);
  turret.changePosition(0, 0);
  turret.moveT();
  Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time, &turret);

  // Check if the detector detects any object.
  detector_model.detect();
  EXPECT_GT(detector_model.objs_vector.size(), 0);

  real_time.release();
  turret.release();
}

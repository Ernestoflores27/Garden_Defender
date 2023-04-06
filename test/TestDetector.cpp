#include <gtest/gtest.h>

#include "../Objects/Detector.h"

// Tests the Detector constructor.
TEST(Detector, Constructor) {
  cv::VideoCapture real_time;
  Turret *turret = new Turret(23, 24, 25);
  std::string modelpath = "Garden_Defender/yoloFastestV2.onnx";
  float objThreshold = 0.5;
  float confThreshold = 0.5;
  float nmsThreshold = 0.5;
  Detector detector(modelpath, objThreshold, confThreshold, nmsThreshold, real_time, turret);

  // Verify the initialized values of the object.
  ASSERT_EQ(detector.objThreshold, objThreshold);
  ASSERT_EQ(detector.confThreshold, confThreshold);
  ASSERT_EQ(detector.nmsThreshold, nmsThreshold);
}

// Tests the Detector detect function.
TEST(Detector, Detect) {
  cv::VideoCapture real_time(0);
  Turret *turret = new Turret(23, 24, 25);
  std::string modelpath = "Garden_Defender/yoloFastestV2.onnx";
  float objThreshold = 0.5;
  float confThreshold = 0.5;
  float nmsThreshold = 0.5;
  Detector detector(modelpath, objThreshold, confThreshold, nmsThreshold, real_time, turret);

  // Verify that the detect function does not crash.
  ASSERT_NO_THROW(detector.detect());
}

// Tests the Detector turretCallback function.
TEST(Detector, TurretCallback) {
  cv::VideoCapture real_time(0);
  Turret *turret = new Turret(23, 24, 25);
  std::string modelpath = "Garden_Defender/yoloFastestV2.onnx";
  float objThreshold = 0.5;
  float confThreshold = 0.5;
  float nmsThreshold = 0.5;
  Detector detector(modelpath, objThreshold, confThreshold, nmsThreshold, real_time, turret);

  // Verify that the turretCallback function does not crash.
  ASSERT_NO_THROW(detector.turretCallback());
}

// Test the persistence function
TEST(DetectorTest, TestPersistence) {
  // Set up the detector
  std::string modelpath = "Garden_Defender/yoloFastestV2.onnx";
  float obj_Threshold = 0.5;
  float conf_Threshold = 0.5;
  float nms_Threshold = 0.5;
  cv::VideoCapture real_time;
  Turret *turret_ = new Turret(23, 24, 25);
  Detector detector(modelpath, obj_Threshold, conf_Threshold, nms_Threshold, real_time, turret_);

  // Set up some objects for testing
  Object obj1(10, 10, 1, 1, 0.8, "person");
  Object obj2(20, 20, 2, 2, 0.9, "car");
  Object obj3(30, 30, 3, 3, 0.7, "person");
  detector.objs_vector = {obj1, obj2, obj3};
  detector.objs_vector_old = {obj1};

  // Call the persistence function and check the filtered objects
  detector.persistence();
  EXPECT_EQ(detector.objs_vector_filtered.size(), 2);
  EXPECT_EQ(detector.objs_vector_filtered[0].center_x, 10);
  EXPECT_EQ(detector.objs_vector_filtered[1].center_x, 30);
}

// Test the getOffsetX and getOffsetY functions
TEST(DetectorTest, TestOffset) {
  // Set up the detector
  std::string modelpath = "Garden_Defender/yoloFastestV2.onnx";
  float obj_Threshold = 0.5;
  float conf_Threshold = 0.5;
  float nms_Threshold = 0.5;
  cv::VideoCapture real_time;
  Turret *turret_ = new Turret(23, 24, 25);
  Detector detector(modelpath, obj_Threshold, conf_Threshold, nms_Threshold, real_time, turret_);

  // Set up an object for testing
  Object obj(10, 20, 1, 1, 0.8, "person");
  detector.objs_vector = {obj};

  // Call the getOffsetX and getOffsetY functions and check the result
  EXPECT_FLOAT_EQ(detector.getOffsetX(), -10);
  EXPECT_FLOAT_EQ(detector.getOffsetY(), -20);
}

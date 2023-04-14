#include <gtest/gtest.h>
#include <pigpiod_if2.h>
#include "../Objects/Servos.cpp"


class ServosTest : public ::testing::Test {
protected:
    Servos servos;
    const float MIN_PWM = 1000.0f;
    const float MAX_PWM = 2000.0f;
    const float MIN_ANGLE = -90.0f;
    const float MAX_ANGLE = 90.0f;
};

TEST_F(ServosTest, DefaultConstructor) {
    // Test default constructor, no setup needed
}

TEST_F(ServosTest, ServoInit) {
    // Test servo initialization
    servos.servoInit(MIN_PWM, MAX_PWM, MIN_ANGLE, MAX_ANGLE);
    // Assert that minimum and maximum PWM values are set correctly
    EXPECT_EQ(MIN_PWM, servos.minPWM);
    EXPECT_EQ(MAX_PWM, servos.maxPWM);
    // Assert that minimum and maximum angle values are set correctly
    EXPECT_EQ(MIN_ANGLE, servos.minAngle);
    EXPECT_EQ(MAX_ANGLE, servos.maxAngle);
}

TEST_F(ServosTest, MapAngleToPWM) {
    // Test mapping of angle to PWM value
    servos.servoInit(MIN_PWM, MAX_PWM, MIN_ANGLE, MAX_ANGLE);
    // Test mapping of minimum angle to minimum PWM value
    EXPECT_EQ(MIN_PWM, servos.mapAngleToPWM(MIN_ANGLE));
    // Test mapping of maximum angle to maximum PWM value
    EXPECT_EQ(MAX_PWM, servos.mapAngleToPWM(MAX_ANGLE));
    // Test mapping of angle in between minimum and maximum angles
    const float ANGLE = -45.0f;
    const float EXPECTED_PWM = MIN_PWM + (((MAX_PWM - MIN_PWM) / (MAX_ANGLE - MIN_ANGLE)) * (ANGLE - MIN_ANGLE));
    EXPECT_EQ(EXPECTED_PWM, servos.mapAngleToPWM(ANGLE));
}

TEST_F(ServosTest, ServoMove) {
    // Test moving of servo to given angle
    servos.servoInit(MIN_PWM, MAX_PWM, MIN_ANGLE, MAX_ANGLE);
    // Test moving servo to minimum angle
    const int IDX = 0;
    const float ANGLE = MIN_ANGLE;
    const float EXPECTED_PWM = servos.mapAngleToPWM(ANGLE);
    // Test moving servo to maximum angle
    const float ANGLE2 = MAX_ANGLE;
    const float EXPECTED_PWM2 = servos.mapAngleToPWM(ANGLE2);
    // Test moving servo to angle in between minimum and maximum angles
    const float ANGLE3 = -45.0f;
    const float EXPECTED_PWM3 = servos.mapAngleToPWM(ANGLE3);
}

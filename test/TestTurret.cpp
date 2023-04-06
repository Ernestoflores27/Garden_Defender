#include "gtest/gtest.h"
#include "Servos.h"
#include "Turret.h"

TEST(TurretTest, testChangePosition)
{
    Turret t(1, 2, 3);
    t.changePosition(10, 20);
    EXPECT_EQ(t.pitch, 10);
    EXPECT_EQ(t.yaw, 20);
}

TEST(TurretTest, testMovePitch)
{
    Turret t(1, 2, 3);
    t.pitch = 0;
    t.movePitch(10);
    EXPECT_EQ(t.pitch, 10);
}

TEST(TurretTest, testMoveYaw)
{
    Turret t(1, 2, 3);
    t.yaw = 0;
    t.moveYaw(20);
    EXPECT_EQ(t.yaw, 20);
}

TEST(TurretTest, testShoot)
{
    Turret t(1, 2, 3);
    t.shoot();
    // expect turret GPIO to be low for 500ms
    // expect shootingLed_GPIO to be high for 500ms
}

TEST(TurretTest, testExplore)
{
    Turret t(1, 2, 3);
    t.pitch = 0;
    t.explore();
    // expect pitch to change by dir value
}

TEST(TurretTest, testTurretTerminate)
{
    Turret t(1, 2, 3);
    t.turretTerminate();
    // expect Turret_GPIO to be high
}

TEST(TurretTest, testClamp)
{
    Turret t(1, 2, 3);
    float val = 10;
    float min = 0;
    float max = 5;
    float res = t.clamp(val, min, max);
    EXPECT_EQ(res, 5);
}

TEST(TurretTest, testResetTime)
{
    Turret t(1, 2, 3);
    t.resetTime();
    // expect start_exploring_time to be current time
}

TEST(TurretTest, testRelease)
{
    Turret t(1, 2, 3);
    t.release();
    // expect all GPIOs to be high
}


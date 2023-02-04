#include <PiPCA9685/PCA9685.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class Servos
{
public:
    PiPCA9685::PCA9685 pca{};
    int minPWM, maxPWM, minAngle, maxAngle;
    Servos()
    {
    }
    void servoInit(int minPWM_, int maxPWM_, int minAngle_, int maxAngle_)
    {
        minPWM = minPWM_;
        maxPWM = maxPWM_;
        minAngle = minAngle_;
        maxAngle = maxAngle_;
        pca.set_pwm_freq(50.0);
    }
    void servoMove(int idx, int angle)
    {
        int pwm = mapAngleToPWM(angle);
        pca.set_pwm(idx, 0, pwm);
    }
    int mapAngleToPWM(int angle)
    {
        return minPWM + (((maxPWM - minPWM) / (maxAngle - minAngle)) * (angle - minAngle));
    }
};
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

class Servos
{
public:
    PiPCA9685::PCA9685 pca{};
    float minPWM, maxPWM, minAngle, maxAngle;
    Servos()
    {
    }
    void servoInit(float minPWM_, float maxPWM_, float minAngle_, float maxAngle_)
    {
        minPWM = minPWM_;
        maxPWM = maxPWM_;
        minAngle = minAngle_;
        maxAngle = maxAngle_;
        pca.set_pwm_freq(50.0);
    }
    void servoMove(int idx, float angle)
    {
        float pwm = mapAngleToPWM(angle);

        pca.set_pwm(idx, 0, pwm);
    }

    float mapAngleToPWM(float angle)
    {
        return minPWM + (((maxPWM - minPWM) / (maxAngle - minAngle)) * (angle - minAngle));
    }
};
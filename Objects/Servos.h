/**

@file Servos.h
@brief A class to control servos using PCA9685.
This class allows to control servos using PCA9685 PWM driver.
It provides a function to initialize the servos and another function to move each servo to a given angle.
*/
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>
/**

@brief A class to control servos using PCA9685 PWM driver.
*/
class Servos
{
public:
    PiPCA9685::PCA9685 pca{};
    
    float minPWM, maxPWM, minAngle, maxAngle;
    /**
     * @brief Default constructor.
     * 
     */
    Servos()
    {
    }
    /**
 * @brief Initializes the servos.
 * 
 * @param minPWM_ The minimum PWM value for the servos.
 * @param maxPWM_ The maximum PWM value for the servos.
 * @param minAngle_ The minimum angle value for the servos.
 * @param maxAngle_ The maximum angle value for the servos.
 */
    void servoInit(float minPWM_, float maxPWM_, float minAngle_, float maxAngle_)
    {
        minPWM = minPWM_;
        maxPWM = maxPWM_;
        minAngle = minAngle_;
        maxAngle = maxAngle_;
        pca.set_pwm_freq(50.0);
    }
    /**
 * @brief Moves a servo to a given angle.
 * 
 * @param idx The index of the servo to move.
 * @param angle The angle to move the servo to.
 */
    void servoMove(int idx, float angle)
    {
        float pwm = mapAngleToPWM(angle);

        pca.set_pwm(idx, 0, pwm);
    }
/**
 * @brief Maps an angle to PWM value.
 * 
 * @param angle The angle to map.
 * @return The corresponding PWM value.
 */
    float mapAngleToPWM(float angle)
    {
        return minPWM + (((maxPWM - minPWM) / (maxAngle - minAngle)) * (angle - minAngle));
    }
};
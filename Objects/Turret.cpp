#include "Turret.h"
/**

@brief Constructor for Turret class.
Initializes the servo and GPIO pins for the turret and shooting LED. Sets the start times for shooting and exploring.
@param GPIO_ GPIO pin number for the turret.
@param shootingLed_GPIO_ GPIO pin number for the shooting LED.
@param lookingLed_GPIO_ GPIO pin number for the looking LED.
@param min_pitch_ Minimum pitch angle.
@param max_pitch_ Maximum pitch angle.
@param min_yaw_ Minimum yaw angle.
@param max_yaw_ Maximum yaw angle.
*/
Turret::Turret(int GPIO_, int shootingLed_GPIO_, int lookingLed_GPIO_, Detector *detector_model_, int min_pitch_, int max_pitch_, int min_yaw_, int max_yaw_)
{
    // Servo Setup
    servos.servoInit(104, 514, -90, 90);
    min_pitch = min_pitch_;
    max_pitch = max_pitch_;
    min_yaw = min_yaw_;
    max_yaw = max_yaw_;
    detector_model = detector_model_;

    // Relay Pin Setup
    Turret_GPIO = GPIO_;
    if (gpioInitialise() < 0)
        return;
    gpioWrite(Turret_GPIO, 1);

    start_shooting_time = time(0);
    start_exploring_time = time(0);

    shootingLed_GPIO = shootingLed_GPIO_;
    lookingLed_GPIO = lookingLed_GPIO_;
    gpioWrite(shootingLed_GPIO, 0);
    gpioWrite(lookingLed_GPIO, 0);
}
/**

@brief Changes the pitch and yaw angles of the turret.
Clamps the pitch and yaw angles to their respective minimum and maximum values.
@param pitch_ New pitch angle.
@param yaw_ New yaw angle.
*/
void Turret::changePosition(int pitch_, int yaw_)
{
    pitch = clamp(pitch_, min_pitch, max_pitch);
    yaw = clamp(yaw_, min_yaw, max_yaw);
}
/**

@brief Moves the pitch angle of the turret by a certain amount.
Clamps the pitch angle to its respective minimum and maximum values.
@param dir Direction to move the pitch angle.
*/
void Turret::movePitch(float dir)
{
    pitch = clamp(pitch + dir, min_pitch, max_pitch);
}
/**

@brief Moves the yaw angle of the turret by a certain amount.
Clamps the yaw angle to its respective minimum and maximum values.
@param dir Direction to move the yaw angle.
*/
void Turret::moveYaw(float dir)
{
    yaw = clamp(yaw + dir, min_yaw, max_yaw);
}
/**

@brief Moves the turret to its new pitch and yaw angles.
*/
void Turret::moveTurret()
{
    servos.servoMove(0, yaw);
    servos.servoMove(1, pitch);
}
/**

@brief Moves the turret to its new pitch and yaw angles in a separate thread.
*/
void Turret::moveT()
{
    std::thread t1(&Turret::moveThread, this);
    t1.detach();
    std::thread t2(&Turret::aimThread, this);
    t2.detach();
}
/**

@brief Thread function for moving the turret.
*/
void Turret::moveThread()
{
    while (true)
    {
        new_pitch = (old_pitch * SMOOTHING) + (pitch * (1 - SMOOTHING));
        new_yaw = (old_yaw * SMOOTHING) + (yaw * (1 - SMOOTHING));
        old_pitch = new_pitch;
        old_yaw = new_yaw;
        servos.servoMove(0, new_yaw);
        servos.servoMove(1, new_pitch);
        std::this_thread::sleep_for(1ms);
    }
}
void Turret::aimThread()
{
    while (true)
    {
        explore();
        if (!detector_model->objs_vector.empty())
        {
            resetTime();

            float error_x = detector_model->getOffsetX() * 2 / 320;
            float error_y = detector_model->getOffsetY() * 2 / 320;

            movePitch(SPEED * error_x);
            moveYaw(-SPEED * error_y);

            if (abs(error_x) < MARGIN and abs(error_y) < MARGIN)
            {
                shoot();
                detector_model->showShooting();
            }
        }
        char c = (char)cv::waitKey(25);
        if (c == 27)
        {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }
}
/**

@brief Shoots the turret by activating the shooting relay.
*/
void Turret::shoot()
{
    if (difftime(time(0), start_shooting_time) > 3)
    {
        std::cout << "Shooting! \n";
        start_shooting_time = time(0);
        std::thread t1(&Turret::shootThread, this);
        t1.detach();
    }
}
/**

@brief Thread function for Shooting the turret by activating the shooting relay.
*/
void Turret::shootThread()
{
    gpioWrite(Turret_GPIO, 0);
    gpioWrite(shootingLed_GPIO, 1);
    std::this_thread::sleep_for(500ms);
    gpioWrite(Turret_GPIO, 1);
    gpioWrite(shootingLed_GPIO, 0);
}
/**

    @brief To search where there is no detection.
    */
void Turret::explore()
{
    if (time(0) - start_exploring_time > 1)
    {
        yaw = 0;
        movePitch(dir);
    }

    if (pitch > 80)
    {
        dir = -2;
    }
    else if (pitch < -80)
    {
        dir = 2;
    }
}
/**

    @brief To terminate the turret to detach GPIO
    */
void Turret::turretTerminate()
{
    gpioWrite(Turret_GPIO, 1);
    gpioTerminate();
}
/**

   @brief Method to keep the values within the range.
   */
float Turret::clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    else if (max < value)
    {
        return max;
    }
    else
        return value;
}
/**

@brief Reset the time from the last time it detected the previous object.
*/
void Turret::resetTime()
{
    start_exploring_time = time(0);
}
/**

    @brief Release resources.
    */
void Turret::release()
{
    gpioTerminate();
}
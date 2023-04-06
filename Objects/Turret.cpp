#include "Turret.h"

Turret::Turret(int GPIO_, int shootingLed_GPIO_, int lookingLed_GPIO_, int min_pitch_, int max_pitch_, int min_yaw_, int max_yaw_)
{
    // Servo Setup
    servos.servoInit(104, 514, -90, 90);
    min_pitch = min_pitch_;
    max_pitch = max_pitch_;
    min_yaw = min_yaw_;
    max_yaw = max_yaw_;

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

void Turret::changePosition(int pitch_, int yaw_)
{
    pitch = clamp(pitch_, min_pitch, max_pitch);
    yaw = clamp(yaw_, min_yaw, max_yaw);
}

void Turret::movePitch(float dir)
{
    pitch = clamp(pitch + dir, min_pitch, max_pitch);
}

void Turret::moveYaw(float dir)
{
    yaw = clamp(yaw + dir, min_yaw, max_yaw);
}

void Turret::moveTurret()
{
    servos.servoMove(0, yaw);
    servos.servoMove(1, pitch);
}

void Turret::moveT()
{
    std::thread t1(&Turret::moveThread, this);
    t1.detach();
}

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

void Turret::shootThread()
{
    gpioWrite(Turret_GPIO, 0);
    gpioWrite(shootingLed_GPIO, 1);
    std::this_thread::sleep_for(500ms);
    gpioWrite(Turret_GPIO, 1);
    gpioWrite(shootingLed_GPIO, 0);
}

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

void Turret::turretTerminate()
{
    gpioWrite(Turret_GPIO, 1);
    gpioTerminate();
}

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

void Turret::resetTime()
{
    start_exploring_time = time(0);
}

void Turret::release()
{
    gpioTerminate();
}
#include <pigpiod_if2.h>
#include "Servos.h"
#include <time.h>
#include <thread>
#include <iostream>

using namespace std;

class Turret
{
public:
    int Turret_GPIO;
    float pitch, yaw, min_pitch, max_pitch, min_yaw, max_yaw;
    float old_pitch = 0, old_yaw = 0, new_pitch = 0, new_yaw = 0;
    float dir = 5;
    Servos servos;
    time_t start_shooting_time, start_exploring_time;

    Turret(int GPIO_, int min_pitch_ = -90, int max_pitch_ = 90, int min_yaw_ = -45, int max_yaw_ = 30)
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
    }
    void changePosition(int pitch_, int yaw_)
    {
        pitch = clamp(pitch_, min_pitch, max_pitch);
        yaw = clamp(yaw_, min_yaw, max_yaw);
    }
    void movePitch(float dir)
    {
        pitch = clamp(pitch + dir, min_pitch, max_pitch);
    }
    void moveYaw(float dir)
    {
        yaw = clamp(yaw + dir, min_yaw, max_yaw);
    }
    void moveTurret()
    {
        servos.servoMove(0, yaw);
        servos.servoMove(1, pitch);
    }
    void moveT()
    {
        thread t1(&Turret::moveThread, this);
        t1.detach();
    }
    void moveThread()
    {
        while (true)
        {
            new_pitch = (old_pitch * 0.97) + (pitch * 0.03);
            new_yaw = (old_yaw * 0.97) + (yaw * 0.03);
            old_pitch = new_pitch;
            old_yaw = new_yaw;
            servos.servoMove(0, new_yaw);
            servos.servoMove(1, new_pitch);
            this_thread::sleep_for(10ms);
        }
    }
    void shoot()
    {
        if (difftime(time(0), start_shooting_time) > 3)
        {
            cout << "Shooting! \n";
            start_shooting_time = time(0);
            thread t1(&Turret::shootThread, this);
            t1.detach();
        }
    }

    void shootThread()
    {
        gpioWrite(Turret_GPIO, 0);
        this_thread::sleep_for(500ms);
        gpioWrite(Turret_GPIO, 1);
    }

    void explore()
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

    void turretTerminate()
    {
        gpioWrite(Turret_GPIO, 1);
        gpioTerminate();
    }

    float clamp(float value, float min, float max)
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
    void resetTime()
    {
        start_exploring_time = time(0);
    }
    void release()
    {
        gpioTerminate();
    }
};
#include <pigpiod_if2.h>
#include "Servos.h"

using namespace std;

class Turret
{
public:
    int Turret_GPIO;
    int pitch, yaw, min_pitch, max_pitch, min_yaw, max_yaw;
    Servos servos;

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
    }
    void changePosition(int pitch_, int yaw_)
    {
        pitch = clamp(pitch_, min_pitch, max_pitch);
        yaw = clamp(yaw_, min_yaw, max_yaw);
        moveTurret();
    }
    void movePitch(int dir)
    {
        pitch = clamp(pitch + dir, min_pitch, max_pitch);
        moveTurret();
    }
    void moveYaw(int dir)
    {
        yaw = clamp(yaw + dir, min_yaw, max_yaw);
        moveTurret();
    }
    void moveTurret()
    {
        servos.servoMove(0, yaw);
        servos.servoMove(1, pitch);
    }
    void shoot()
    {
        gpioWrite(Turret_GPIO, 0);
        // usleep(500000);
        gpioWrite(Turret_GPIO, 1);
    }
    void explore()
    {
    }
    void turretTerminate()
    {
        gpioTerminate();
    }

    int clamp(int value, int min, int max)
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
};
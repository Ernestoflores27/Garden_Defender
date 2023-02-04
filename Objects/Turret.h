#include <pigpiod_if2.h>
#include "./Servos.h"

using namespace std;

class Turret
{
public:
    int Turret_GPIO;
    int pitch, yaw;
    Servos servos;

    Turret(int GPIO_)
    {
        // Servo Setup
        servos.servoInit(204, 410, -90, 90);

        // Relay Pin Setup
        Turret_GPIO = GPIO_;
        if (gpioInitialise() < 0)
            return;
        gpioWrite(Turret_GPIO, 1);
    }
    void changePosition(int pitch_, int yaw_)
    {
        pitch = pitch_;
        yaw = yaw_;
        moveTurret();
    }
    void moveTurret()
    {
        servos.servoMove(0, yaw);
        servos.servoMove(1, pitch);
    }
    void movePitch(int dir)
    {
        pitch = pitch + dir;
        moveTurret();
    }
    void moveYaw(int dir)
    {
        yaw = yaw + dir;
        moveTurret();
    }
    void shoot()
    {
        gpioWrite(Turret_GPIO, 0);
        usleep(500000);
        gpioWrite(Turret_GPIO, 1);
    }
    void explore()
    {
        // for (int pos = 204; pos < 410; pos++)
        // {
        //     pca.set_pwm(0, 0, pos);
        //     usleep(10'000);
        // }
        // for (int pos = 410; pos > 204; pos--)
        // {
        //     pca.set_pwm(0, 0, pos);
        //     usleep(10'000);
        // }
        // for (int pos = 204; pos < 410; pos++)
        // {
        //     pca.set_pwm(1, 0, pos);
        //     usleep(10'000);
        // }
        // for (int pos = 410; pos > 204; pos--)
        // {
        //     pca.set_pwm(1, 0, pos);
        //     usleep(10'000);
        // }
    }
    void turretTerminate()
    {
        gpioTerminate();
    }
};
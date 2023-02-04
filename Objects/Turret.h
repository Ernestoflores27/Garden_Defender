#include <pigpiod_if2.h>
#include "./Servos.h"

using namespace std;

class Turret
{
public:
    int Turret_GPIO;
    Servos servos;

    Turret(int GPIO_)
    {
        servos.servoInit(204, 410, -90, 90);
        servos.servoMove(0, 0);

        // Relay Pin Setup
        Turret_GPIO = GPIO_;
        if (gpioInitialise() < 0)
            return;
        gpioWrite(Turret_GPIO, 1);
    }

        void moveTurret()
    {
    }
    void moveHorizontal(int dir)
    {
        printf("moveHorizontal\n");
    }
    void moveVertical(int dir)
    {
        printf("moveVertical\n");
    }
    void shoot()
    {
        printf("Shooting\n");

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
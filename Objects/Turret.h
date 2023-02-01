#include <pigpiod_if2.h>
#include <unistd.h>

using namespace std;

#define LED_PIN 6

class Turret
{
public:
    int Turret_GPIO;
    Turret(int GPIO_)
    {
        Turret_GPIO = GPIO_;
        if (gpioInitialise() < 0)
            return;
        gpioWrite(Turret_GPIO, 1);
    }
    void turretTerminate()
    {
        gpioTerminate();
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
    }
};
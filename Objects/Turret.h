#include <pigpio.h>

using namespace std;

#define LED_PIN 6

class Turret
{
public:
    Turret()
    {
        gpioInitialise();
        gpioSetMode(LED_PIN, PI_OUTPUT);
    }
    void MoveTurret()
    {
    }
    void MoveHorizontal()
    {
    }
    void MoveVertical()
    {
    }
    void Shoot()
    {
        while (true)
        {
            gpioDelay(100000);
            gpioWrite(LED_PIN, 1);
            gpioDelay(100000);
            gpioWrite(LED_PIN, 0);
        }
    }
    void Explore()
    {
    }
};
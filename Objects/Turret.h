#include <pigpiod_if2.h>
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

using namespace std;

class Turret
{
public:
    int Turret_GPIO;
    Turret(int GPIO_)
    {
        // PCA9685 SETUP
        PiPCA9685::PCA9685 pca{};
        pca.set_pwm_freq(50.0);

        for (int pos = 150; pos < 460; pos++)
        {
            pca.set_pwm(0, 0, pos);
            usleep(1'000);
        }
        for (int pos = 460; pos > 150; pos--)
        {
            pca.set_pwm(0, 0, pos);
            usleep(1'000);
        }
        for (int pos = 150; pos < 460; pos++)
        {
            pca.set_pwm(1, 0, pos);
            usleep(1'000);
        }
        for (int pos = 460; pos > 150; pos--)
        {
            pca.set_pwm(1, 0, pos);
            usleep(1'000);
        }

        // Relay Pin Setup
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
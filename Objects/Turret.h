/**
@file Turret.h
@brief Class definition for the Turret class. It controls the turrent position by modifying the pitch and yaw.
*/
#include <pigpiod_if2.h>
#include "Servos.cpp"
#include <time.h>
#include <thread>

using namespace std::chrono_literals;
#define SMOOTHING 0.9
#define MARGIN 0.2
#define SPEED 1

/**
@brief Turret class definition.
*/
class Turret
{
public:
	bool manual = false;
	bool shooting = false;
	bool running = false;
	std::thread *worker_1 = nullptr;
	std::thread *worker_2 = nullptr;
	int Turret_GPIO;
	int shootingLed_GPIO;
	int lookingLed_GPIO;
	float pitch, yaw, min_pitch, max_pitch, min_yaw, max_yaw;
	float old_pitch = 0, old_yaw = 0, new_pitch = 0, new_yaw = 0;
	float dir = 5;
	Servos servos;
	time_t start_shooting_time, start_exploring_time, start_aiming_time;
	std::vector<Object> objs_vector;

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
	Turret(int GPIO_, int shootingLed_GPIO_, int lookingLed_GPIO_, int min_pitch_ = -90, int max_pitch_ = 90, int min_yaw_ = -45, int max_yaw_ = 30);

	/**
	@brief Changes the pitch and yaw angles of the turret.
	Clamps the pitch and yaw angles to their respective minimum and maximum values.
	@param pitch_ New pitch angle.
	@param yaw_ New yaw angle.
	*/
	void changePosition(int pitch_, int yaw_);

	/**
	@brief Moves the pitch angle of the turret by a certain amount.
	Clamps the pitch angle to its respective minimum and maximum values.
	@param dir Direction to move the pitch angle.
	*/
	void movePitch(float dir);

	/**
	@brief Moves the yaw angle of the turret by a certain amount.
	Clamps the yaw angle to its respective minimum and maximum values.
	@param dir Direction to move the yaw angle.
	*/
	void moveYaw(float dir);

	/**
	@brief Moves the turret to its new pitch and yaw angles.
	*/
	void moveTurret();

	/**
	@brief Moves the turret to its new pitch and yaw angles in a separate thread.
	*/
	void start();

	/**
	@brief Thread function for moving the turret.
	*/
	void moveThread();

	/**
	@brief Thread function for aiming the turret.
	*/
	void aimThread();

	/**
	@brief Shoots the turret by activating the shooting relay.
	*/
	void shoot();

	/**
	 @brief Thread function for Shooting the turret by activating the shooting relay.
	*/
	void shootThread();

	/**
	@brief To search where there is no detection.
	*/
	void explore();

	/**
	@brief To terminate the turret
	*/
	void turretTerminate();

	/**
	@brief To terminate the turret to detach GPIO
	*/
	float clamp(float value, float min, float max);

	/**
	@brief Reset the time from the last time it detected the previous object.
	*/
	void resetTime();

	/**
	@brief Release resources.
	*/
	void stop();
};
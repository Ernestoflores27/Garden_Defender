#include "Objects/Camera.cpp"
#include "Objects/Detector.cpp"
#include "Objects/Turret.cpp"
#include "Objects/GUI.cpp"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>
#include <iostream>

class Callback_Detector : public Detector::Callback_Detector
{
public:
	Turret *turret;
	Callback_Detector(Turret *turret_)
	{
		turret = turret_;
	}
	void callback_func(std::vector<Object> &objs_vector)
	{
		turret->objs_vector = objs_vector;
	}
};

class Callback_GUI : public GUI::Callback_GUI
{
public:
	Turret *turret;
	Callback_GUI(Turret *turret_)
	{
		turret = turret_;
	}
	void callback_func(int k)
	{
		if (k == 113)
			turret->manual = !turret->manual;
		else if (k == 82)
			turret->moveYaw(-5);
		else if (k == 84)
			turret->moveYaw(5);
		else if (k == 81)
			turret->movePitch(5);
		else if (k == 83)
			turret->movePitch(-5);
		else if (k == 32)
			turret->shoot();
	}
};

int main()
{
	// Init camera
	cv::Mat video_stream;
	Camera cam1(640, 480);
	cv::VideoCapture real_time = cam1.getVideoCapture();

	// Init Turret
	Turret turret(23, 24, 25);
	turret.changePosition(0, 0);
	turret.start();

	// Init Detector
	Callback_Detector ca_detector(&turret);
	Detector detector_model("Garden_Defender/Objects/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time);
	detector_model.registerCallback(&ca_detector);
	detector_model.start();

	// Start GUI
	Callback_GUI ca_gui(&turret);
	GUI gui(&detector_model, &turret);
	gui.registerCallback(&ca_gui);
	gui.start();

	// Stop everything
	detector_model.stop();
	turret.stop();
}

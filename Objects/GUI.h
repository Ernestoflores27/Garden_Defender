/**

@file GUI.h
@brief Header file containing the declaration of references needed in the GUI class.
*/
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
// #include "Object.cpp"
#include <iostream>
#include <thread>

/**

@class GUI
@brief Class that handles object detection using a neural network.
*/
class GUI
{
public:
	/*

	@brief Constructor for GUI.
	@param turret Pointer to the Turret object.
	*/
	GUI(Detector *detector_model_, Turret *turret_);

	/**
	@brief Method for showing the frame with detected objects.
	*/
	void show();

	/**
	@brief Method for showing the frame with detected objects and the crosshair on the turret.
	*/
	void showShooting();

	/**
	@brief OpenCV Mat object containing the current frame.
	*/
	cv::Mat frame;

	/**
	@brief Callback interface which needs to be implemented by the user.
	**/
	struct Callback_GUI
	{
		virtual void callback_func(int k) = 0;
	};
	/**
	 * Register the callback interface here to receive data.
	 **/
	void registerCallback(Callback_GUI *ca)
	{
		callback = ca;
	}

private:
	/**
	 * @brief Input width of the YOLOv5 model.
	 */
	const int inpWidth = 352;
	/**
	 * @brief Input height of the YOLOv5 model.
	 */
	const int inpHeight = 352;
	/**
	 * @brief Draws a crosshair in the center of a given frame for tracking.
	 */
	void drawCrossair(cv::Mat &frame);
	/**
	 * @brief Draws a green line between the center of the detected object and the center of the video frame for tracking.
	 */
	void lineClosest(cv::Mat &frame);

	void showThread();
	Detector *detector_model;
	Callback_GUI *callback = nullptr;
	Turret *turret;
};

GUI::GUI(Detector *detector_model_, Turret *turret_)
{
	this->turret = turret_;
	this->detector_model = detector_model_;
}

/**
@brief Method to draw a crosshair in the center of a given frame for tracking.
*/
void GUI::drawCrossair(cv::Mat &frame)
{
	cv::Size s = frame.size();
	cv::line(frame, cv::Point(s.width / 2, 0), cv::Point(s.width / 2, s.height), cv::Scalar(0, 255, 0), 1);
	cv::line(frame, cv::Point(0, s.height / 2), cv::Point(s.width, s.height / 2), cv::Scalar(0, 255, 0), 1);
}

/**
@brief Method draws a green line between the center of the detected object and the center of the video frame for tracking.
*/
void GUI::lineClosest(cv::Mat &frame)
{
	cv::Size s = frame.size();
	if (detector_model->objs_vector.empty())
		return;

	line(frame, cv::Point(s.width / 2, s.height / 2), cv::Point(detector_model->objs_vector[0].center_x, detector_model->objs_vector[0].center_y), cv::Scalar(0, 255, 0), 2);
}

/**
@brief Method for showing the frame with detected objects and the crosshair on the turret.
*/
void GUI::showShooting()
{
	putText(this->frame, "Shooting", cv::Point(detector_model->objs_vector[0].x, detector_model->objs_vector[0].y + 25), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
}

/**
@brief Method for showing the frame with detected objects and the information to command the turret in manual or automatic mode. It also command the turrent when it is in Manual mode.
*/
void GUI::show()
{
	while (true)
	{
		frame = detector_model->frame;
		if (!frame.empty())
		{
			this->drawCrossair(frame);
			this->lineClosest(frame);
			if (!detector_model->objs_vector.empty())
			{
				for (Object object : detector_model->objs_vector)
				{
					circle(frame, cv::Point(object.center_x, object.center_y), 5, cv::Scalar(255, 0, 0), -1);
					circle(frame, cv::Point(object.center_x, object.center_y), 30, cv::Scalar(255, 0, 0));
				}
			}

			if (turret->shooting)
			{
				putText(this->frame, "Shooting", cv::Point(detector_model->objs_vector[0].center_x, detector_model->objs_vector[0].center_y - 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
			}

			if (!turret->manual)
			{
				putText(this->frame, "Mode: Auto", cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
				putText(this->frame, "To change modes press Q", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
			}
			else
			{
				putText(this->frame, "Mode: Manual", cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
				putText(this->frame, "To change modes press Q", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
				putText(this->frame, "To move use arrow keys", cv::Point(10, 60), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
				putText(this->frame, "To shoot press the space key", cv::Point(10, 80), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
			}

			static const std::string kWinName = "Garden Defender";
			cv::namedWindow(kWinName);
			cv::imshow(kWinName, frame);
			int k = cv::waitKey(100);

			if (k != 0 && nullptr != this->callback)
			{
				this->callback->callback_func(k);
			}
		}
	}
}

void GUI::showT()
{
	std::thread t1(&GUI::show, this);
	t1.detach();
}
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
	@param modelpath Path to the model file.
	@param objThreshold Object detection threshold.
	@param confThreshold Confidence threshold.
	@param nmsThreshold NMS threshold.
	@param real_time OpenCV VideoCapture object for real-time video capture.
	@param turret Pointer to the Turret object.
	*/
	GUI(cv::VideoCapture real_time, Detector *detector_model_);
	/**

@brief Method for detecting objects in a frame and showing them with a crosshair on the turret.
*/
	void detectT();
	/**

@brief Method for showing the frame with detected objects.
*/
	void show();
	/**

@brief Method for showing the frame with detected objects and the crosshair on the turret.
*/
	void showShooting();
	/**

@brief Vector of Object objects containing the detected objects.
*/
	std::vector<Object> objs_vector;
	/**

@brief OpenCV VideoCapture object for real-time video capture.
*/
	cv::VideoCapture real_time;
	/**

@brief OpenCV Mat object containing the current frame.
*/
	cv::Mat frame;

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
	 * @brief Mark if it is in Manual mode
	 */
	bool manual = false;
	/**
	 * @brief Draws a crosshair in the center of a given frame for tracking.
	 */
	void drawCrossair(cv::Mat &frame);
	/**
	 * @brief Draws a green line between the center of the detected object and the center of the video frame for tracking.
	 */
	void lineClosest(cv::Mat &frame);
	/**
	 * @brief Draw a rectangle displaying the bounding box and display it
	 */
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame);
	void detectThread();
	Detector *detector_model;
};

GUI::GUI(cv::VideoCapture real_time, Detector *detector_model_)
{
	this->real_time = real_time;
	this->detector_model = detector_model_;
}
/**

@brief Method to draws predicted bounding boxes on the frame.
*/
void GUI::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame)
{
	// Draw a rectangle displaying the bounding box
	rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 255), 2);

	// // Get the label for the class name and its confidence
	// std::string label = cv::format("%.2f", conf);
	// label = this->classes[classId] + ":" + label;

	// // Display the label at the top of the bounding box
	// int baseLine;
	// cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_DUPLEX, 0.5, 1, &baseLine);
	// top = std::max(top, labelSize.height);
	// cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 1.5);
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
	if (objs_vector.empty())
		return;

	line(frame, cv::Point(s.width / 2, s.height / 2), cv::Point(objs_vector[0].center_x, objs_vector[0].center_y), cv::Scalar(0, 255, 0), 2);
}
/**

@brief Method for showing the frame with detected objects and the crosshair on the turret.
*/
void GUI::showShooting()
{
	putText(this->frame, "Shooting", cv::Point(objs_vector[0].x, objs_vector[0].y + 25), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
}

/**

@brief Method for showing the frame with detected objects and the information to command the turret in manual or automatic mode. It also command the turrent when it is in Manual mode.
*/
void GUI::show()
{
	real_time.read(frame);
	objs_vector = detector_model->objs_vector;
	if (!frame.empty())
	{
		this->drawCrossair(frame);
		this->lineClosest(frame);

		if (manual == false)
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
		cv::namedWindow(kWinName, cv::WINDOW_NORMAL);
		cv::setWindowProperty(kWinName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
		cv::resize(frame, frame, cv::Size(frame.cols * 2, frame.rows * 2), 0, 0, cv::INTER_LINEAR);
		cv::imshow(kWinName, frame);
		int k = cv::waitKey(1);

		// if (k == 82)
		// {
		// 	turret->moveYaw(-5);
		// }
		// else if (k == 84)
		// {
		// 	turret->moveYaw(5);
		// }
		// else if (k == 81)
		// {
		// 	turret->movePitch(5);
		// }
		// else if (k == 83)
		// {
		// 	turret->movePitch(-5);
		// }
		// else if (k == 113)
		// {
		// 	manual = !manual;
		// }
		// else if (k == 32)
		// {
		// 	turret->shoot();
		// }
		// else
		// {
		// }
	}
}
/**

@brief Method that calls the detect() function. It works in a separete thread without interrupting other operations in the program.
*/
void GUI::detectThread()
{
	while (true)
	{
		this->show();
	}
}

void GUI::detectT()
{
	std::thread t1(&GUI::detectThread, this);
	t1.detach();
}
/**
@file Detector.h
@brief Header file containing the declaration of references needed in the Detector class.
*/
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/dnn.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Object.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

/**
@class Detector
@brief Class that handles object detection using a neural network.
*/
class Detector
{
public:
	/*
	@brief Constructor for Detector.
	@param modelpath Path to the model file.
	@param objThreshold Object detection threshold.
	@param confThreshold Confidence threshold.
	@param nmsThreshold NMS threshold.
	@param real_time OpenCV VideoCapture object for real-time video capture.
	@param turret Pointer to the Turret object.
	*/
	Detector(std::string modelpath, float objThreshold, float confThreshold, float nmsThreshold, cv::VideoCapture real_time);

	/**
	@brief Method for detecting objects in a frame and showing them with a crosshair on the turret.
	*/
	void detectT();

	/**
	@brief Method for detecting objects in a frame and showing them.
	*/
	void detect();

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

	/**
	@brief Callback interface which needs to be implemented by the user.
	**/
	struct Callback_Detector
	{
		virtual void callback_func(std::vector<Object> &objs_vector) = 0;
	};
	/**
	 * Register the callback interface here to receive data.
	 **/
	void registerCallback(Callback_Detector *ca)
	{
		callback = ca;
	}

private:
	Callback_Detector *callback = nullptr;
	/**
	 * @brief Array of anchor values for YOLOv5 model.
	 */
	const float anchors[2][6] = {{12.64, 19.39, 37.88, 51.48, 55.71, 138.31}, {126.91, 78.23, 131.57, 214.55, 279.92, 258.87}};
	/**
	 * @brief Array of stride values for YOLOv5 model.
	 */
	const float stride[3] = {16.0, 32.0};
	/**
	 * @brief Input width of the YOLOv5 model.
	 */
	const int inpWidth = 352;
	/**
	 * @brief Input height of the YOLOv5 model.
	 */
	const int inpHeight = 352;
	/**
	 * @brief Number of stages in the YOLOv3 model.
	 */
	const int num_stage = 2;
	/**
	 * @brief Number of anchor values in the YOLOv3 model.
	 */
	const int anchor_num = 3;
	/**
	 * @brief Minimum value for detection
	 */
	float objThreshold;
	/**
	 * @brief Confidence threshold
	 */
	float confThreshold;
	/**
	 * @brief Minimum value for classification
	 */
	float nmsThreshold;
	/**
	 * @brief Mark if it is in Manual mode
	 */
	bool manual = false;
	/**
	 * @brief Variable that stores the possible classes
	 */
	std::vector<std::string> classes;
	/**
	 * @brief Classifier file path
	 */
	const std::string classesFile = "Garden_Defender/object_detection.txt";
	/**
	 * @brief Amount of classes in the classification
	 */
	int num_class;
	/**
	 * @brief Neural network
	 */
	cv::dnn::Net net;
	/**
	 * @brief Arrange the detected objects to calculate the closest to the center.
	 */
	void sortObjs();
	/**
	 * @brief The thread to detect
	 */
	void detectThread();
	/**
	 * @brief Improves the object detection by appliying a filter to the recognition.
	 */
	void persistence();
};
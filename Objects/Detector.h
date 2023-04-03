/**

@file Detector.h
@brief Header file containing the declaration of the Detector class.
*/
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/dnn.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Object.h"
#include "Turret.h"
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
	Detector(std::string modelpath, float objThreshold, float confThreshold, float nmsThreshold, cv::VideoCapture real_time, Turret *turret);
	/**

@brief Method for detecting objects in a frame and showing them with a crosshair on the turret.
*/
	void detectT();
	/**

@brief Method for detecting objects in a frame and showing them.
*/
	void detect();
	/**

@brief Method for getting the x-offset of the detected object from the center of the frame.
@return Float value of the x-offset.
*/
	float getOffsetX();
	/**

@brief Method for getting the y-offset of the detected object from the center of the frame.
@return Float value of the y-offset.
*/
	float getOffsetY();
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
	std::vector<Object> objs_vector, objs_vector_old, objs_vector_filtered;
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
* @brief Pointer to the Turret object.
*/
	Turret *turret;
	/**
 * @brief Callback function to control the turret.
 */
	void turretCallback();
	/**
 * @brief Array of anchor values for YOLOv3 model.
 */
	const float anchors[2][6] = {{12.64, 19.39, 37.88, 51.48, 55.71, 138.31}, {126.91, 78.23, 131.57, 214.55, 279.92, 258.87}};
/**
 * @brief Array of stride values for YOLOv3 model.
 */
	const float stride[3] = {16.0, 32.0};
	/**
 * @brief Input width of the YOLOv3 model.
 */
	const int inpWidth = 352;
	/**
 * @brief Input height of the YOLOv3 model.
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
 * @brief 
 */
	float objThreshold;
		/**
 * @brief c
 */
	float confThreshold;
		/**
 * @brief 
 */
	float nmsThreshold;
		/**
 * @brief mark if it is manual
 */
	bool manual = false;
		/**
 * @brief 
 */
	std::vector<std::string> classes;
		/**
 * @brief 
 */
	const std::string classesFile = "Garden_Defender/object_detection.txt";
		/**
 * @brief 
 */
	int num_class;
		/**
 * @brief 
 */
	cv::dnn::Net net;
		/**
 * @brief 
 */
	void sortObjs();
		/**
 * @brief 
 */
	void drawCrossair(cv::Mat &frame);
		/**
 * @brief 
 */
	void lineClosest(cv::Mat &frame);
		/**
 * @brief The thread to detect
 */
	void detectThread();
		/**
 * @brief 
 */
	void persistence();
		/**
	 * @brief Draw a rectangle displaying the bounding box and display it
	 */
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame);
};
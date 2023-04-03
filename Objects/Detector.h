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

class Detector
{
public:
	Detector(std::string modelpath, float objThreshold, float confThreshold, float nmsThreshold, cv::VideoCapture real_time, Turret *turret);
	void detectT();
	void detect();
	float getOffsetX();
	float getOffsetY();
	void show();
	void showShooting();
	std::vector<Object> objs_vector, objs_vector_old, objs_vector_filtered;
	cv::VideoCapture real_time;
	cv::Mat frame;

private:
	Turret *turret;
	void turretCallback();
	const float anchors[2][6] = {{12.64, 19.39, 37.88, 51.48, 55.71, 138.31}, {126.91, 78.23, 131.57, 214.55, 279.92, 258.87}};
	const float stride[3] = {16.0, 32.0};
	const int inpWidth = 352;
	const int inpHeight = 352;
	const int num_stage = 2;
	const int anchor_num = 3;
	float objThreshold;
	float confThreshold;
	float nmsThreshold;
	bool manual = false;
	std::vector<std::string> classes;
	const std::string classesFile = "Garden_Defender/object_detection.txt";
	int num_class;
	cv::dnn::Net net;
	void sortObjs();
	void drawCrossair(cv::Mat &frame);
	void lineClosest(cv::Mat &frame);
	void detectThread();
	void persistence();
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame);
};
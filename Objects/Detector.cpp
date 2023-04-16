#include "Detector.h"

/*
@brief Constructor for Detector.
@param modelpath Path to the model file.
@param objThreshold Object detection threshold.
@param confThreshold Confidence threshold.
@param nmsThreshold NMS threshold.
@param real_time OpenCV VideoCapture object for real-time video capture.
@param turret Pointer to the Turret object.
*/
Detector::Detector(std::string modelpath, float obj_Threshold, float conf_Threshold, float nms_Threshold, cv::VideoCapture real_time)
{
	this->real_time = real_time;
	this->objThreshold = obj_Threshold;
	this->confThreshold = conf_Threshold;
	this->nmsThreshold = nms_Threshold;

	std::ifstream ifs(this->classesFile.c_str());
	std::string line;
	while (getline(ifs, line))
		this->classes.push_back(line);
	this->num_class = this->classes.size();
	this->net = cv::dnn::readNet(modelpath);
}

/**
@brief Method for detecting objects in a frame and showing them. Performs non-maximum suppression to eliminate redundant overlapping boxes with lower confidences. Draws predicted bounding boxes on the frame and saves them to the vector.
*/
void Detector::detect()
{
	real_time >> frame;
	cv::Mat blob;
	cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(this->inpWidth, this->inpHeight));
	this->net.setInput(blob);
	std::vector<cv::Mat> outs;
	this->net.forward(outs, this->net.getUnconnectedOutLayersNames());

	// generate proposals
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	float ratioh = (float)frame.rows / this->inpHeight, ratiow = (float)frame.cols / this->inpWidth;
	int n = 0, q = 0, i = 0, j = 0, nout = this->anchor_num * 5 + this->classes.size(), row_ind = 0;
	float *pdata = (float *)outs[0].data;
	for (n = 0; n < this->num_stage; n++)
	{
		int num_grid_x = (int)(this->inpWidth / this->stride[n]);
		int num_grid_y = (int)(this->inpHeight / this->stride[n]);
		for (i = 0; i < num_grid_y; i++)
		{
			for (j = 0; j < num_grid_x; j++)
			{
				cv::Mat scores = outs[0].row(row_ind).colRange(this->anchor_num * 5, outs[0].cols);
				cv::Point classIdPoint;
				double max_class_socre;
				// Get the value and location of the maximum score
				cv::minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
				for (q = 0; q < this->anchor_num; q++)
				{
					const float anchor_w = this->anchors[n][q * 2];
					const float anchor_h = this->anchors[n][q * 2 + 1];
					float box_score = pdata[4 * this->anchor_num + q];
					if (box_score > this->objThreshold && max_class_socre > this->confThreshold)
					{
						float cx = (pdata[4 * q] * 2.f - 0.5f + j) * this->stride[n];
						float cy = (pdata[4 * q + 1] * 2.f - 0.5f + i) * this->stride[n];
						float w = powf(pdata[4 * q + 2] * 2.f, 2.f) * anchor_w;
						float h = powf(pdata[4 * q + 3] * 2.f, 2.f) * anchor_h;

						int left = (cx - 0.5 * w) * ratiow;
						int top = (cy - 0.5 * h) * ratioh;

						classIds.push_back(classIdPoint.x);
						confidences.push_back(box_score * max_class_socre);
						boxes.push_back(cv::Rect(left, top, (int)(w * ratiow), (int)(h * ratioh)));
						// TODO: Add variable in if statement to variable to check status
					}
				}
				row_ind++;
				pdata += nout;
			}
		}
	}

	// Perform non maximum suppression to eliminate redundant overlapping boxes with lower confidences
	objs_vector.clear();
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];

		if (classIds[idx] == 0)
		{
			cv::Rect box = boxes[idx];
			cv::Size s = frame.size();
			Object f(box, s.width, s.height);
			this->objs_vector.push_back(f);
		}
	}
	this->sortObjs();
	if (nullptr != this->callback)
	{
		this->callback->callback_func(this->objs_vector);
	}
}

/**
@brief Method to arrange the detected objects to calculate the closest to the center.
*/
void Detector::sortObjs()
{
	sort(objs_vector.begin(), objs_vector.end());
}

/**
@brief Method that calls the detect() function. It works in a separete thread without interrupting other operations in the program.
*/
void Detector::run()
{
	while (true)
	{
		this->detect();
	}
}

/**
@brief Method to enable a new thread for detection and and detach it from the main thread.
*/
void Detector::start()
{
	std::thread t1(&Detector::run, this);
	t1.detach();
}
/**

@file Detector.cpp
@brief Header file containing the declaration of the Detector class.
*/
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
Detector::Detector(std::string modelpath, float obj_Threshold, float conf_Threshold, float nms_Threshold, cv::VideoCapture real_time, Turret *turret_)
{
	this->real_time = real_time;
	this->objThreshold = obj_Threshold;
	this->confThreshold = conf_Threshold;
	this->nmsThreshold = nms_Threshold;
	this->turret = turret_;

	std::ifstream ifs(this->classesFile.c_str());
	std::string line;
	while (getline(ifs, line))
		this->classes.push_back(line);
	this->num_class = this->classes.size();
	this->net = cv::dnn::readNet(modelpath);
}
	/**

@brief Method for detecting objects in a frame and showing them.
*/
void Detector::detect()
{
	real_time.read(frame);
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
			this->drawPred(classIds[idx], confidences[idx], box.x, box.y,
						   box.x + box.width, box.y + box.height, frame);
			cv::Size s = frame.size();
			Object f(box, s.width, s.height);
			this->objs_vector.push_back(f);
		}
	}
	this->sortObjs();
	this->show();
	if (manual == false)
		turretCallback();
}
/**
 * @brief Callback function to control the turret.
 */
void Detector::turretCallback()
{
	turret->explore();
	if (!objs_vector.empty())
	{
		turret->resetTime();

		float error_x = getOffsetX() * 2 / 320;
		float error_y = getOffsetY() * 2 / 320;

		turret->movePitch(SPEED * error_x);
		turret->moveYaw(-SPEED * error_y);

		if (abs(error_x) < MARGIN and abs(error_y) < MARGIN)
		{
			turret->shoot();
			showShooting();
		}
	}
}
void Detector::persistence()
{
	if (objs_vector.empty())
		return;

	int dist_margin = 50;
	objs_vector_filtered.clear();

	for (Object obj_old : objs_vector_old)
	{
		for (Object obj : objs_vector)
		{
			int offset_x = obj_old.center_x - obj.center_x;
			int offset_y = obj_old.center_y - obj.center_y;
			int distance = (offset_x * offset_x) + (offset_y * offset_y);

			if (distance < dist_margin * dist_margin)
			{
				objs_vector_filtered.push_back(obj);
			}
		}
	}

	for (Object face_old : objs_vector_old)
	{
		circle(frame, cv::Point(face_old.center_x, face_old.center_y), 5, cv::Scalar(0, 0, 255), -1);
	}
	for (Object object : objs_vector)
	{
		circle(frame, cv::Point(object.center_x, object.center_y), 5, cv::Scalar(255, 0, 0), -1);
	}
	for (Object object : objs_vector_filtered)
	{
		circle(frame, cv::Point(object.center_x, object.center_y), dist_margin, cv::Scalar(255, 0, 0));
	}

	objs_vector_old = objs_vector;
}

/**

@brief Method for getting the x-offset of the detected object from the center of the frame.
@return Float value of the x-offset.
*/
float Detector::getOffsetX()
{
	return this->objs_vector[0].offset_x;
}
	/**

@brief Method for getting the y-offset of the detected object from the center of the frame.
@return Float value of the y-offset.
*/
float Detector::getOffsetY()
{
	return this->objs_vector[0].offset_y;
}

void Detector::sortObjs()
{
	sort(objs_vector.begin(), objs_vector.end());
}

void Detector::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame)
{
	// Draw a rectangle displaying the bounding box
	rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 255), 2);

	// Get the label for the class name and its confidence
	std::string label = cv::format("%.2f", conf);
	label = this->classes[classId] + ":" + label;

	// Display the label at the top of the bounding box
	int baseLine;
	cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_DUPLEX, 0.5, 1, &baseLine);
	top = std::max(top, labelSize.height);
	cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 1.5);
}

void Detector::drawCrossair(cv::Mat &frame)
{
	cv::Size s = frame.size();
	cv::line(frame, cv::Point(s.width / 2, 0), cv::Point(s.width / 2, s.height), cv::Scalar(0, 255, 0), 1);
	cv::line(frame, cv::Point(0, s.height / 2), cv::Point(s.width, s.height / 2), cv::Scalar(0, 255, 0), 1);
}

void Detector::lineClosest(cv::Mat &frame)
{
	cv::Size s = frame.size();
	if (objs_vector.empty())
		return;

	line(frame, cv::Point(s.width / 2, s.height / 2), cv::Point(objs_vector[0].center_x, objs_vector[0].center_y), cv::Scalar(0, 255, 0), 2);
}
/**

@brief Method for showing the frame with detected objects and the crosshair on the turret.
*/
void Detector::showShooting()
{
	putText(this->frame, "Shooting", cv::Point(objs_vector[0].x, objs_vector[0].y + 25), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
}

/**

@brief Method for showing the frame with detected objects.
*/
void Detector::show()
{
	if (!frame.empty())
	{
		this->drawCrossair(frame);
		this->lineClosest(frame);
		this->persistence();

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

		if (k == 82)
		{
			turret->moveYaw(-5);
		}
		else if (k == 84)
		{
			turret->moveYaw(5);
		}
		else if (k == 81)
		{
			turret->movePitch(5);
		}
		else if (k == 83)
		{
			turret->movePitch(-5);
		}
		else if (k == 113)
		{
			manual = !manual;
		}
		else if (k == 32)
		{
			turret->shoot();
		}
		else
		{
		}
	}
}

void Detector::detectThread()
{
	while (true)
	{
		this->detect();
		// run callback
	}
}
	/**

@brief Method for detecting objects in a frame and showing them with a crosshair on the turret.
*/
void Detector::detectT()
{
	std::thread t1(&Detector::detectThread, this);
	t1.detach();
}

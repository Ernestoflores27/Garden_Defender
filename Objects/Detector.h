#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/dnn.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include "Face.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace dnn;
using namespace std;

// Constants.
const float INPUT_WIDTH = 352;
const float INPUT_HEIGHT = 352;
const float SCORE_THRESHOLD = 0.3;
const float NMS_THRESHOLD = 0.4;
const float CONFIDENCE_THRESHOLD = 0.3;

// Text parameters.
const float FONT_SCALE = 0.4;
const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
const int THICKNESS = 1;

// Colors.
Scalar BLACK = Scalar(0, 0, 0);
Scalar BLUE = Scalar(255, 178, 50);
Scalar YELLOW = Scalar(0, 255, 255);
Scalar RED = Scalar(0, 0, 255);

class Detector
{
public:
	Mat video_stream;
	CascadeClassifier faceDetector;
	VideoCapture real_time;
	vector<Rect> faces;
	vector<Face> faces_vector;

	// Load class list.
	vector<string> class_list;
	ifstream ifs;
	string line;

	Net net;
	vector<Mat> detections;
	Mat img;

	Detector(VideoCapture real_time_)
	{
		real_time = real_time_;

		ifs = ifstream("Garden_Defender/object_detection.txt");
		while (getline(ifs, line))
		{
			class_list.push_back(line);
		}
		net = readNetFromONNX("Garden_Defender/yolov5n.onnx");

		namedWindow("Object Detection");
	}

	vector<Face> Detect()
	{
		real_time.read(video_stream);
		detections = pre_process(video_stream, net);
		vector<Face> a = post_process(video_stream.clone(), detections, class_list);

		vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double t = net.getPerfProfile(layersTimes) / freq;
		string label = format("Inference time : %.2f ms", t);

		putText(video_stream, label, Point(20, 40), FONT_FACE, FONT_SCALE, RED);
		return a;
	}

	vector<Mat> pre_process(Mat &input_image, Net &net)
	{
		Mat blob;
		blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);
		net.setInput(blob);
		vector<Mat> outputs;
		net.forward(outputs, net.getUnconnectedOutLayersNames());
		return outputs;
	}

	vector<Face> post_process(Mat input_image, vector<Mat> &outputs, const vector<string> &class_name)
	{
		vector<int> class_ids;
		vector<float> confidences;
		vector<Rect> boxes;

		float x_factor = input_image.cols / INPUT_WIDTH;
		float y_factor = input_image.rows / INPUT_HEIGHT;
		float *data = (float *)outputs[0].data;
		const int dimensions = 85;
		const int rows = 2 * x_factor * y_factor;

		cout << "rows: ";
		cout << rows;
		cout << "\n";

		for (int i = 0; i < rows; ++i)
		{
			float confidence = data[4];

			cout << "confidence: ";
			cout << confidence;
			cout << "\n";
			if (confidence >= CONFIDENCE_THRESHOLD)
			{

				float *classes_scores = data + 5;
				Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
				Point class_id;
				double max_class_score;

				minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

				cout << "max_class_score: ";
				cout << max_class_score;
				cout << "\n";
				if (max_class_score > SCORE_THRESHOLD)
				{

					confidences.push_back(confidence);

					class_ids.push_back(class_id.x);

					cout << "confidence: ";
					cout << confidence;
					cout << ", class_id: ";
					cout << class_id;
					cout << "\n";

					float cx = data[0];
					float cy = data[1];
					float w = data[2];
					float h = data[3];
					int left = int((cx - 0.5 * w) * x_factor);
					int top = int((cy - 0.5 * h) * y_factor);
					int width = int(w * x_factor);
					int height = int(h * y_factor);
					boxes.push_back(Rect(left, top, width, height));
				}
			}
			data += 85;
		}

		vector<int> indices;
		NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);

		faces_vector.clear();
		for (int i = 0; i < indices.size(); i++)
		{
			int idx = indices[i];
			Rect box = boxes[idx];
			Face f(box, real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT));
			faces_vector.push_back(f);
		}
		sortFaces();
		return faces_vector;
	}

	void draw_label(Mat &input_image, string label, int left, int top)
	{
		int baseLine;
		Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
		top = max(top, label_size.height);
		Point tlc = Point(left, top);
		Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
		rectangle(input_image, tlc, brc, BLACK, FILLED);
		putText(input_image, label, Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
	}

	void Show()
	{
		imshow("Object Detection", video_stream);
	}

	void sortFaces()
	{
		sort(faces_vector.begin(), faces_vector.end());
	}
	int getOffsetX()
	{
		return faces_vector[0].offset_x;
	}
	int getOffsetY()
	{
		return faces_vector[0].offset_y;
	}
	void drawBoundaries()
	{
		sort(faces_vector.begin(), faces_vector.end());
		int i = 0;
		for (Face f : faces_vector)
		{
			Scalar c;
			if (i == 0)
			{
				c = Scalar(255, 0, 255);
			}
			else
			{
				c = Scalar(0, 255, 0);
			}
			string position_text = to_string(f.offset_x) + ", " + to_string(f.offset_y);
			rectangle(video_stream, Point(f.x, f.y), Point(f.w, f.h), c, 2, 8, 0);
			i++;
		}
	}

	// void drawCrossair()
	// {
	// 	line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, 0), Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT)), Scalar(0, 0, 255), 1);
	// 	line(video_stream, Point(0, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Scalar(0, 0, 255), 1);
	// }
	// void lineClosest()
	// {
	// 	line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(faces_vector[0].center_x, faces_vector[0].center_y), Scalar(0, 255, 0), 2);
	// }
	void showShooting()
	{
		putText(video_stream, "Shooting", Point(faces_vector[0].x, faces_vector[0].y - 5), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 0, 255), 1.5);
	}
};

class yolo_fast
{
public:
	yolo_fast(string modelpath, float objThreshold, float confThreshold, float nmsThreshold);
	vector<Face> detect(Mat &srcimg);
	void sortFaces();
	int getOffsetX();
	int getOffsetY();

private:
private:
	vector<Face> faces_vector;

	const float anchors[2][6] = {{12.64, 19.39, 37.88, 51.48, 55.71, 138.31}, {126.91, 78.23, 131.57, 214.55, 279.92, 258.87}};
	const float stride[3] = {16.0, 32.0};
	const int inpWidth = 352;
	const int inpHeight = 352;
	const int num_stage = 2;
	const int anchor_num = 3;
	float objThreshold;
	float confThreshold;
	float nmsThreshold;
	vector<string> classes;
	const string classesFile = "Garden_Defender/object_detection.txt";
	int num_class;
	Net net;
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame);
};

yolo_fast::yolo_fast(string modelpath, float obj_Threshold, float conf_Threshold, float nms_Threshold)
{
	this->objThreshold = obj_Threshold;
	this->confThreshold = conf_Threshold;
	this->nmsThreshold = nms_Threshold;

	ifstream ifs(this->classesFile.c_str());
	string line;
	while (getline(ifs, line))
		this->classes.push_back(line);
	this->num_class = this->classes.size();
	this->net = readNet(modelpath);
}

void yolo_fast::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame) // Draw the predicted bounding box
{
	// Draw a rectangle displaying the bounding box
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 2);

	// Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	label = this->classes[classId] + ":" + label;

	// Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	// rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 1);
}

vector<Face> yolo_fast::detect(Mat &frame)
{
	Mat blob;
	blobFromImage(frame, blob, 1 / 255.0, Size(this->inpWidth, this->inpHeight));
	this->net.setInput(blob);
	vector<Mat> outs;
	this->net.forward(outs, this->net.getUnconnectedOutLayersNames());

	/////generate proposals
	vector<int> classIds;
	vector<float> confidences;
	vector<Rect> boxes;
	float ratioh = (float)frame.rows / this->inpHeight, ratiow = (float)frame.cols / this->inpWidth;
	int n = 0, q = 0, i = 0, j = 0, nout = this->anchor_num * 5 + this->classes.size(), row_ind = 0;
	float *pdata = (float *)outs[0].data;
	for (n = 0; n < this->num_stage; n++) /// stage
	{
		int num_grid_x = (int)(this->inpWidth / this->stride[n]);
		int num_grid_y = (int)(this->inpHeight / this->stride[n]);
		for (i = 0; i < num_grid_y; i++)
		{
			for (j = 0; j < num_grid_x; j++)
			{
				Mat scores = outs[0].row(row_ind).colRange(this->anchor_num * 5, outs[0].cols);
				Point classIdPoint;
				double max_class_socre;
				// Get the value and location of the maximum score
				minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
				for (q = 0; q < this->anchor_num; q++) /// anchor
				{
					const float anchor_w = this->anchors[n][q * 2];
					const float anchor_h = this->anchors[n][q * 2 + 1];
					float box_score = pdata[4 * this->anchor_num + q];
					if (box_score > this->objThreshold && max_class_socre > this->confThreshold)
					{
						float cx = (pdata[4 * q] * 2.f - 0.5f + j) * this->stride[n];	  /// cx
						float cy = (pdata[4 * q + 1] * 2.f - 0.5f + i) * this->stride[n]; /// cy
						float w = powf(pdata[4 * q + 2] * 2.f, 2.f) * anchor_w;			  /// w
						float h = powf(pdata[4 * q + 3] * 2.f, 2.f) * anchor_h;			  /// h

						int left = (cx - 0.5 * w) * ratiow;
						int top = (cy - 0.5 * h) * ratioh; /// ���껹ԭ��ԭͼ��

						classIds.push_back(classIdPoint.x);
						confidences.push_back(box_score * max_class_socre);
						boxes.push_back(Rect(left, top, (int)(w * ratiow), (int)(h * ratioh)));
					}
				}
				row_ind++;
				pdata += nout;
			}
		}
	}

	// Perform non maximum suppression to eliminate redundant overlapping boxes with
	// lower confidences
	faces_vector.clear();
	vector<int> indices;
	NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{

		int idx = indices[i];

		Rect box = boxes[idx];
		this->drawPred(classIds[idx], confidences[idx], box.x, box.y,
					   box.x + box.width, box.y + box.height, frame);

		if (classIds[idx] == 0)
		{
			Face f(box, 320, 240);
			this->faces_vector.push_back(f);
		}
	}
	this->sortFaces();
	return this->faces_vector;
}

void yolo_fast::sortFaces()
{
	sort(faces_vector.begin(), faces_vector.end());
}
int yolo_fast::getOffsetX()
{
	return this->faces_vector[0].offset_x;
}
int yolo_fast::getOffsetY()
{
	return this->faces_vector[0].offset_y;
}
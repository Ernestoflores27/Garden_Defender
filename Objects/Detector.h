#include <opencv4/opencv2/opencv.hpp>
#include "Face.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;

// Text parameters.
const float FONT_SCALE = 0.7;
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

	Detector(VideoCapture real_time_)
	{
		real_time = real_time_;

		ifs = ifstream("Garden_Defender/object_detection.txt");
		while (getline(ifs, line))
		{
			class_list.push_back(line);
		}
		net = readNet("Garden_Defender/yolov5s.onnx");

		string trained_classifier_location = "Garden_Defender/haarcascade_frontalface_default.xml";
		faceDetector.load(trained_classifier_location);

		namedWindow("Face Detection");
	}

	void draw_label(Mat &input_image, string label, int left, int top)
	{
		// Display the label at the top of the bounding box.
		int baseLine;
		Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
		top = max(top, label_size.height);
		// Top left corner.
		Point tlc = Point(left, top);
		// Bottom right corner.
		Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
		// Draw white rectangle.
		rectangle(input_image, tlc, brc, BLACK, FILLED);
		// Put the label on the black rectangle.
		putText(input_image, label, Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
	}

	vector<Rect> Detect()
	{
		real_time.read(video_stream);
		faceDetector.detectMultiScale(video_stream, faces, 1.1, 4, CASCADE_SCALE_IMAGE, Size(30, 30));

		faces_vector.clear();
		for (Rect face : faces)
		{
			Face f(face, real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT));
			faces_vector.push_back(f);
		}
		sortFaces();
		return faces;
	}
	vector<Mat> DetectObj()
	{
		real_time.read(video_stream);
		detections = pre_process(video_stream, net);
		Mat img = post_process(video_stream.clone(), detections, class_list);

		vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double t = net.getPerfProfile(layersTimes) / freq;
		string label = format("Inference time : %.2f ms", t);

		putText(img, label, Point(20, 40), FONT_FACE, FONT_SCALE, RED);
		imshow("Output", img);
		return detections;
	}

	void drawBoundaries()
	{
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
			// putText(video_stream, position_text, Point(f.x, f.y - 5), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 0, 255), 1.5);
			rectangle(video_stream, Point(f.x, f.y), Point(f.w, f.h), c, 2, 8, 0);
			i++;
		}
	}

	// void drawCrossair()
	// {
	// 	line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, 0), Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT)), Scalar(0, 0, 255), 1);
	// 	line(video_stream, Point(0, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Scalar(0, 0, 255), 1);
	// }
	void draw_label_(Mat &input_image, string label, int left, int top)
	{
		// Display the label at the top of the bounding box.
		int baseLine;
		Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
		top = max(top, label_size.height);
		// Top left corner.
		Point tlc = Point(left, top);
		// Bottom right corner.
		Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
		// Draw white rectangle.
		rectangle(input_image, tlc, brc, BLACK, FILLED);
		// Put the label on the black rectangle.
		putText(input_image, label, Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
	}

	vector<Mat> pre_process(Mat &input_image, Net &net)
	{
		// Convert to blob.
		Mat blob;
		blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

		net.setInput(blob);

		// Forward propagate.
		vector<Mat> outputs;
		net.forward(outputs, net.getUnconnectedOutLayersNames());

		return outputs;
	}

	Mat post_process(Mat input_image, vector<Mat> &outputs, const vector<string> &class_name)
	{
		// Initialize vectors to hold respective outputs while unwrapping detections.
		vector<int> class_ids;
		vector<float> confidences;
		vector<Rect> boxes;
		// Resizing factor.
		float x_factor = input_image.cols / INPUT_WIDTH;
		float y_factor = input_image.rows / INPUT_HEIGHT;
		float *data = (float *)outputs[0].data;
		const int dimensions = 85;
		// 25200 for default size 640.
		const int rows = 25200;
		// Iterate through 25200 detections.
		for (int i = 0; i < rows; ++i)
		{
			float confidence = data[4];
			// Discard bad detections and continue.
			if (confidence >= CONFIDENCE_THRESHOLD)
			{
				float *classes_scores = data + 5;
				// Create a 1x85 Mat and store class scores of 80 classes.
				Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
				// Perform minMaxLoc and acquire the index of best class  score.
				Point class_id;
				double max_class_score;
				minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
				// Continue if the class score is above the threshold.
				if (max_class_score > SCORE_THRESHOLD)
				{
					// Store class ID and confidence in the pre-defined respective vectors.
					confidences.push_back(confidence);
					class_ids.push_back(class_id.x);
					// Center.
					float cx = data[0];
					float cy = data[1];
					// Box dimension.
					float w = data[2];
					float h = data[3];
					// Bounding box coordinates.
					int left = int((cx - 0.5 * w) * x_factor);
					int top = int((cy - 0.5 * h) * y_factor);
					int width = int(w * x_factor);
					int height = int(h * y_factor);
					// Store good detections in the boxes vector.
					boxes.push_back(Rect(left, top, width, height));
				}
			}
			// Jump to the next row.
			data += 85;
		}
		// Perform Non-Maximum Suppression and draw predictions.
		vector<int> indices;
		NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
		for (int i = 0; i < indices.size(); i++)
		{
			int idx = indices[i];
			Rect box = boxes[idx];
			int left = box.x;
			int top = box.y;
			int width = box.width;
			int height = box.height;
			// Draw bounding box.
			rectangle(input_image, Point(left, top), Point(left + width, top + height), BLUE, 3 * THICKNESS);
			// Get the label for the class name and its confidence.
			string label = format("%.2f", confidences[idx]);
			label = class_name[class_ids[idx]] + ":" + label;
			// Draw class labels.
			draw_label_(input_image, label, left, top);
		}
		return input_image;
	}

	void Show()
	{
		imshow("Face Detection", video_stream);
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
	// void lineClosest()
	// {
	// 	line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(faces_vector[0].center_x, faces_vector[0].center_y), Scalar(0, 255, 0), 2);
	// }
	void showShooting()
	{
		putText(video_stream, "Shooting", Point(faces_vector[0].x, faces_vector[0].y - 5), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 0, 255), 1.5);
	}
};
#include <opencv4/opencv2/opencv.hpp>
#include "Face.h"
#include <iostream>

using namespace std;

class Detector
{
public:
	Mat video_stream;
	CascadeClassifier faceDetector;
	VideoCapture real_time;
	vector<Rect> faces;
	vector<Face> faces_vector;

	Detector(VideoCapture real_time_)
	{
		real_time = real_time_;

		string trained_classifier_location = "Garden_Defender/haarcascade_frontalface_default.xml";
		faceDetector.load(trained_classifier_location);

		namedWindow("Face Detection");
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

	void drawCrossair()
	{
		line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, 0), Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT)), Scalar(0, 0, 255), 1);
		line(video_stream, Point(0, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Scalar(0, 0, 255), 1);
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
	void lineClosest()
	{
		line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(faces_vector[0].center_x, faces_vector[0].center_y), Scalar(0, 255, 0), 2);
	}
	void showShooting()
	{
		putText(video_stream, "Shooting", Point(faces_vector[0].x, faces_vector[0].y - 5), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 0, 255), 1.5);
	}
};
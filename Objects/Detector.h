#include <opencv4/opencv2/opencv.hpp>

using namespace std;

class Detector
{
public:
	Mat video_stream;
	CascadeClassifier faceDetector;
	VideoCapture real_time;
	vector<Rect> faces;

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
		return faces;
	}

	void Show()
	{
		imshow("Face Detection", video_stream);
	}

	void drawCrossair()
	{
		line(video_stream, Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, 0), Point(real_time.get(CAP_PROP_FRAME_WIDTH) / 2, real_time.get(CAP_PROP_FRAME_HEIGHT)), Scalar(0, 0, 255), 1);
		line(video_stream, Point(0, real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Point(real_time.get(CAP_PROP_FRAME_WIDTH), real_time.get(CAP_PROP_FRAME_HEIGHT) / 2), Scalar(0, 0, 255), 1);
	}

	void drawBoundaries(vector<Rect> faces)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			Mat faceROI = video_stream(faces[i]);

			int x = faces[i].x;
			int y = faces[i].y;
			int h = y + faces[i].height;
			int w = x + faces[i].width;

			int center_x = (x + w) / 2;
			int center_y = (y + h) / 2;

			int offset_x = (real_time.get(CAP_PROP_FRAME_WIDTH) / 2) - center_x;
			int offset_y = (real_time.get(CAP_PROP_FRAME_HEIGHT) / 2) - center_y;

			string position_text = to_string(offset_x) + ", " + to_string(offset_y);
			putText(video_stream, position_text, Point(x, y - 5), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 0, 255), 1.5);
			rectangle(video_stream, Point(x, y), Point(w, h), Scalar(255, 0, 255), 2, 8, 0);
		}
	}
};
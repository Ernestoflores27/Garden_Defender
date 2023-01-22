using namespace std;
#include <opencv2\opencv.hpp>

class Detector
{
public:
	Mat video_stream;
	CascadeClassifier faceDetector; // Declaring an object named 'face detector' of CascadeClassifier class//
	VideoCapture real_time;
	vector<Rect> faces; // Declaring a rectangular vector named faces//

	Detector(VideoCapture real_time_)
	{
		real_time = real_time_;
		// Declaring a matrix hold frames from video stream//

		string trained_classifier_location = "C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"; // Defining the location our XML Trained Classifier in a string//
		faceDetector.load(trained_classifier_location);																// loading the XML trained classifier in the object//

		namedWindow("Face Detection"); // Declaring a window to show the result//
	}

	void Detect()
	{
		faceDetector.detectMultiScale(video_stream, faces, 1.1, 4, CASCADE_SCALE_IMAGE, Size(30, 30)); // Detecting the faces in 'image_with_humanfaces' matrix//
		real_time.read(video_stream);																   // reading frames from camera and loading them in 'video_stream' Matrix//

		for (int i = 0; i < faces.size(); i++)
		{																					 // for locating the face
			Mat faceROI = video_stream(faces[i]);											 // Storing face in the matrix//
			int x = faces[i].x;																 // Getting the initial row value of face rectangle's starting point//
			int y = faces[i].y;																 // Getting the initial column value of face rectangle's starting point//
			int h = y + faces[i].height;													 // Calculating the height of the rectangle//
			int w = x + faces[i].width;														 // Calculating the width of the rectangle//
			rectangle(video_stream, Point(x, y), Point(w, h), Scalar(255, 0, 255), 2, 8, 0); // Drawing a rectangle using around the faces//
		}
		imshow("Face Detection", video_stream);
	}
};
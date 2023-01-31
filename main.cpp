#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include <opencv4/opencv2/opencv.hpp>

int main()
{
    Camera cam1(320, 240);
    VideoCapture real_time = cam1.getVideoCapture();

    Detector detector(real_time);
    vector<Rect> faces;

    while (true)
    {
        faces = detector.Detect();
        detector.drawCrossair();
        detector.drawBoundaries(faces);
        detector.Show();

        if (waitKey(10) == 27)
        {
            break;
        }
    }
}

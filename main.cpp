#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include <opencv2\opencv.hpp>

int main()
{
    Camera cam1;
    VideoCapture real_time = cam1.getVideoCapture();
    Detector detector(real_time);

    while (true)
    {
        detector.Detect();
        detector.Show();

        if (waitKey(10) == 27)
        {
            break;
        }
    }
}

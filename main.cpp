#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include <opencv4/opencv2/opencv.hpp>

#include <pigpiod_if2.h>

using namespace std;

int main()
{
    int GPIO = 18;
    int level;
    if (gpioInitialise() < 0)
        return 1;
    gpioWrite(GPIO, 1);
    level = gpioRead(GPIO);
    printf("GPIO %d is %d\n", GPIO, level);
    gpioTerminate();

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

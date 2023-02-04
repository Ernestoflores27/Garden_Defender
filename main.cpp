#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

int main()
{
    Camera cam1(320, 240);
    VideoCapture real_time = cam1.getVideoCapture();

    Detector detector(real_time);
    vector<Rect> faces;

    Turret turret(18);
    turret.changePosition(-90, 0);
    usleep(1'000'000);
    turret.changePosition(90, 0);
    usleep(1'000'000);
    turret.changePosition(0, -90);
    usleep(1'000'000);
    turret.changePosition(0, 90);
    usleep(1'000'000);

    turret.shoot();

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

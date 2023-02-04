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
    turret.changePosition(0, 0);

    while (true)
    {
        faces = detector.Detect();
        detector.drawBoundaries();
        detector.drawCrossair();

        if (!detector.faces_vector.empty())
        {
            detector.lineClosest();
            if (detector.getOffsetX() > 20)
            {
                turret.movePitch(3);
            }
            else if (detector.getOffsetX() < -20)
            {
                turret.movePitch(-3);
            }

            if (detector.getOffsetY() > 20)
            {
                turret.moveYaw(2);
            }
            else if (detector.getOffsetY() < -20)
            {
                turret.moveYaw(-2);
            }

            if (detector.getOffsetY() < 20 and detector.getOffsetY() > -20 and detector.getOffsetX() < 20 and detector.getOffsetX() > -20)
            {
                turret.shoot();
                detector.showShooting();
            }
        }

        detector.Show();

        int k = waitKey(10);
        if (k == 27)
        {
            break;
        }
        else if (k == 82)
        {
            turret.moveYaw(5);
        }
        else if (k == 84)
        {
            turret.moveYaw(-5);
        }
        else if (k == 81)
        {
            turret.movePitch(5);
        }
        else if (k == 83)
        {
            turret.movePitch(-5);
        }
        else
        {
        }
    }
}

#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

#define MARGIN 40

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
            if (detector.getOffsetX() > MARGIN)
            {
                turret.movePitch(2);
            }
            else if (detector.getOffsetX() < -MARGIN)
            {
                turret.movePitch(-2);
            }

            if (detector.getOffsetY() > MARGIN)
            {
                turret.moveYaw(-1);
            }
            else if (detector.getOffsetY() < -MARGIN)
            {
                turret.moveYaw(1);
            }

            if (detector.getOffsetY() < MARGIN and detector.getOffsetY() > -MARGIN and detector.getOffsetX() < MARGIN and detector.getOffsetX() > -MARGIN)
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
            turret.moveYaw(-5);
        }
        else if (k == 84)
        {
            turret.moveYaw(5);
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

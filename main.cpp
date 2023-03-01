#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

#define MARGIN 15

int main()
{
    Mat video_stream;
    Camera cam1(640, 480);
    VideoCapture real_time = cam1.getVideoCapture();

    yolo_fast yolo_model("Garden_Defender/yolov5n.onnx", 0.3, 0.4, 0.4, real_time);
    vector<Face> faces;

    Turret turret(23);
    turret.changePosition(0, 0);
    turret.moveT();

    while (true)
    {

        faces = yolo_model.detect();

        if (!yolo_model.faces_vector.empty())
        {
            turret.resetTime();
            if (yolo_model.getOffsetX() > MARGIN)
            {
                turret.movePitch(3 * ((yolo_model.getOffsetX() * 4 / 320) + 0.3));
            }
            else if (yolo_model.getOffsetX() < -MARGIN)
            {
                turret.movePitch(3 * ((yolo_model.getOffsetX() * 4 / 320) - 0.3));
            }

            if (yolo_model.getOffsetY() > MARGIN)
            {
                turret.moveYaw(3 * ((yolo_model.getOffsetY() * 4 / 320) - 0.3));
            }
            else if (yolo_model.getOffsetY() < -MARGIN)
            {
                turret.moveYaw(3 * ((yolo_model.getOffsetX() * 4 / 320) + 0.3));
            }

            if (yolo_model.getOffsetY() < MARGIN and yolo_model.getOffsetY() > -MARGIN and yolo_model.getOffsetX() < MARGIN and yolo_model.getOffsetX() > -MARGIN)
            {
                // turret.shoot();
                yolo_model.showShooting();
            }
        }
        else
        {
            turret.explore();
        }

        yolo_model.show();

        int k = waitKey(1);
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

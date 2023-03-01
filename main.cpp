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
    yolo_model.detectT();

    Turret turret(23);
    turret.changePosition(0, 0);
    turret.moveT();

    while (true)
    {
        if (!yolo_model.faces_vector.empty())
        {
            turret.resetTime();
            if (yolo_model.getOffsetX() > MARGIN or yolo_model.getOffsetX() < -MARGIN)
            {
                turret.movePitch(yolo_model.getOffsetX() * 4 / 320);
            }

            if (yolo_model.getOffsetY() > MARGIN or yolo_model.getOffsetY() < -MARGIN)
            {
                turret.moveYaw(-yolo_model.getOffsetY() * 4 / 320);
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

        this_thread::sleep_for(50ms);
    }
}

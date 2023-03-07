#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

#define MARGIN 0.2

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
        turret.explore();
        if (!yolo_model.faces_vector.empty())
        {
            turret.resetTime();

            float error_x = yolo_model.getOffsetX() * 2 / 320;
            float error_y = yolo_model.getOffsetY() * 2 / 320;

            turret.movePitch(2 * error_x);
            turret.moveYaw(-2 * error_y);

            if (abs(error_x) < MARGIN and abs(error_y) < MARGIN)
            {
                turret.shoot();
                yolo_model.showShooting();
            }
        }

        char c = (char)waitKey(25); // Allowing 25 milliseconds frame processing time and initiating break condition//

        if (c == 27)
        { // If 'Esc' is entered break the loop//
            break;
        }

        this_thread::sleep_for(50ms);
    }

    real_time.release();
    turret.release();
}

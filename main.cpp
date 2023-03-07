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

    Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time);
    // thread t1(&Detector::detectThread, detector_model);
    // t1.detach();
    detector_model.detectT();

    Turret turret(23);
    turret.changePosition(0, 0);
    turret.moveT();

    while (true)
    {
        turret.explore();
        if (!detector_model.objs_vector.empty())
        {
            turret.resetTime();

            float error_x = detector_model.getOffsetX() * 2 / 320;
            float error_y = detector_model.getOffsetY() * 2 / 320;

            turret.movePitch(2 * error_x);
            turret.moveYaw(-2 * error_y);

            if (abs(error_x) < MARGIN and abs(error_y) < MARGIN)
            {
                turret.shoot();
                detector_model.showShooting();
            }
        }

        char c = (char)waitKey(25);
        if (c == 27)
        {
            break;
        }
        this_thread::sleep_for(50ms);
    }

    real_time.release();
    turret.release();
}

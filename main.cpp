#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>





int main()
{
    //Declare variable video stream (type: matrix from OpenCV library)
    cv::Mat video_stream;
    Camera cam1(640, 480);
    cv::VideoCapture real_time = cam1.getVideoCapture();

    Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time);
    // detector_model.set_callback();
    detector_model.detectT();

    Turret turret(23);
    turret.changePosition(0, 0);
    turret.moveT();

    while (true)
    {

        turret.turret_move(detector_model);

        char c = (char)cv::waitKey(25);
        if (c == 27)
        {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    real_time.release();
    turret.release();
}
